#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <exception>
#include "common.h"
#include <oolua/oolua.h>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <math.h>
#include <time.h>
#ifndef __arm__
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Kstar.h"

#define RT_KSTAR   5276

char * const cmd_G1 = "G1<cr>"; 	//UPS实时数据G1
char * const cmd_G2 = "G2<cr>"; 	//UPS状态查询G2
char * const cmd_G3 = "G3<cr>"; 	//UPS三相实时数据 G3
char * const cmd_I	= "I<cr>";		//UPS 信息命令 I
char * const cmd_GF = "GF<cr>"; 	//UPS 额定信息 GF
char * const cmd_Q1 = "Q1<cr>"; 	//状态查询 Q1
char * const cmd_F	= "F<cr>";		//UPS 额定信息 F

KSTAR::KSTAR()
{
    device_type_ = "ea86";
    baud_rate_ = 2400;
    memset(&cData, 0x00, sizeof(KSTAR_t));
    mCmdStatus = KSTAR_IDLE;
}

KSTAR::~KSTAR()
{
}

bool KSTAR::InitSetting(const Json::Value& settingRoot )
{
    cData.data_id = data_id_;

    return true;
}

void KSTAR::RoundDone()
{
    if(!bIsDataReady_)
        bIsDataReady_ = true;

    if(buffer_ == nullptr) {
        buffer_len_ = sizeof(cData);
        buffer_ = std::make_unique<uint8_t[]>(buffer_len_);
    }
    mCmdStatus = KSTAR_IDLE;
    SMDSPDevice::Reset();
    cData.update_time = get_ttime(boost::posix_time::second_clock::local_time());

    uint8_t *p = buffer_.get();
    memcpy(p, &cData, sizeof(cData));
    SendSP(RT_EA86, buffer_.get(), sizeof(cData));

    int rvSize = ruleValueVec.size();
    CheckThresholdWork([self(shared_from_this()), this, rvVec(std::move(ruleValueVec))] {
        bool bSave = DoBatchCheckThreshold(rvVec);
#ifndef __arm__
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastSaveTime;

    if(diff.total_seconds() > MAXSAVETIMEINTERVAL || bSave ) {
        using namespace bsoncxx;
        using bsoncxx::builder::basic::kvp;

        lastSaveTime = now; //lastSaveTime will be now
        auto doc = builder::basic::document{};

        doc.append(kvp("data_id", types::b_int64 {data_id_}));
        doc.append(kvp("origin_data", types::b_binary{binary_sub_type::k_binary, sizeof(cData), (uint8_t *)&cData}));
        doc.append(kvp("added_datetime", bsoncxx::types::b_date(std::chrono::system_clock::now())));
        std::string strTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
        std::string strDate = strTime.substr(0, 4) + "-" + strTime.substr(4, 2) + "-" + strTime.substr(6, 2);
        std::string sTime = strTime.substr(9, 2) + ":" + strTime.substr(11, 2) + ":" + strTime.substr(13, 2);
        doc.append(kvp("Date", (strDate)));
        doc.append(kvp("Time", (sTime)));

        SaveMongoObject(device_type_, doc);
    }
#endif
        });
    ruleValueVec.reserve(rvSize);


}

bool KSTAR::process_payload(unsigned char * data, size_t len)
{
    if(data[0] != '0') {
        return false;
    }

    if(EA86_G1 == mCmdStatus) {
        memcpy(&cData.CMD_G1, data, len);
        mCmdStatus = EA86_G2;
        dataNeed = sizeof(cData.CMD_G2);
        SendData((unsigned char *)&cmd_G2, strlen(cmd_G2));
    } else if(EA86_G2 == mCmdStatus) {
        memcpy(&cData.CMD_G2, data, len);

        mCmdStatus = EA86_G3;
        dataNeed = sizeof(cData.CMD_G3);
        SendData((unsigned char *)&cmd_G3, strlen(cmd_G3));
    } else if(EA86_G3 == mCmdStatus) {
        memcpy(&cData.CMD_G3, data, len);

        mCmdStatus = EA86_I;
        dataNeed = sizeof(cData.CMD_I);
        SendData((unsigned char *)&cmd_I, strlen(cmd_I));
    } else if(EA86_I == mCmdStatus) {
        memcpy(&cData.CMD_I, data, len);

        dataNeed = sizeof(cData.CMD_GF);
        mCmdStatus = EA86_GF;
        SendData((unsigned char *)&cmd_GF, strlen(cmd_GF));
    } else if(EA86_GF == mCmdStatus) {
        memcpy(&cData.CMD_GF, data, len);

        dataNeed = sizeof(cData.CMD_Q1);
        mCmdStatus = EA86_QI;
        SendData((unsigned char *)&cmd_Q1, strlen(cmd_Q1));
    } else if(EA86_QI == mCmdStatus) {
        memcpy(&cData.CMD_Q1, data, len);

        mCmdStatus = EA86_F;
        dataNeed = sizeof(cData.CMD_F);
        SendData((unsigned char *)&cmd_F, strlen(cmd_F));
    } else if(EA86_F == mCmdStatus) {
        memcpy(&cData.CMD_F, data, len);
        RoundDone();
        return false;
    }

    return true;
}

bool KSTAR::process(unsigned char * data, size_t len)
{
    if( (msg_index + len) > 4096) {
        Reset();
        return false;
    }

    bool ret = true;

    memcpy(msg_buf + msg_index, data, len);
    msg_index += len;

    if(msg_index >= dataNeed) {
        ret = process_payload(msg_buf, dataNeed);
        msg_index = 0;
    }
    return ret;
}

bool KSTAR::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    mCmdStatus = EA86_G1;
    dataNeed == sizeof(cData.CMD_G1) ;
    SendData((unsigned char *)&cmd_G1, strlen(cmd_G1));
    return true;
}

float KSTAR::Get_Value(uint32_t data_id, const std::string &var_name) const
{
    if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");

    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }

    if(var_name == "motherVoltage") {
        return std::stoi(std::string(cData.CMD_G1.motherVoltage, 3));
    } else if(var_name == "BPhaseVoltage") {
        return std::stoi(std::string(cData.CMD_G1.chargeCurrent, 3));
    } else if(var_name == "CPhaseVoltage_1") {
        return std::stoi(std::string(cData.CMD_G3.ThreePhaseInputVoltage, 5));
    } else if(var_name == "CPhaseVoltage_2") {
        return std::stoi(std::string(&cData.CMD_G3.ThreePhaseInputVoltage[6], 5));
    } else if(var_name == "CPhaseVoltage_3") {
        return std::stoi(std::string(cData.CMD_G3.ThreePhaseInputVoltage[12], 5));
    } else if(var_name == "ThreeBypassInputVoltage_1") {
        return std::stoi(std::string(cData.CMD_G3.ThreeBypassInputVoltage, 5));
    } else if(var_name == "ThreeBypassInputVoltage_2") {
        return std::stoi(std::string(cData.CMD_G3.ThreeBypassInputVoltage[6], 5));
    } else if(var_name == "ThreeBypassInputVoltage_3") {
        return std::stoi(std::string(cData.CMD_G3.ThreeBypassInputVoltage[12], 5));
    } else if(var_name == "OutputVoltage_1") {
        return std::stoi(std::string(cData.CMD_G3.OutputVoltage, 5));
    } else if(var_name == "OutputVoltage_2") {
        return std::stoi(std::string(cData.CMD_G3.OutputVoltage[6], 5));
    } else if(var_name == "OutputVoltage_3") {
        return std::stoi(std::string(cData.CMD_G3.OutputVoltage[12], 5));
    }
    throw std::out_of_range("不支持变量");
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<EA86Provider>());
    return std::move(providerVec);
}

#endif
