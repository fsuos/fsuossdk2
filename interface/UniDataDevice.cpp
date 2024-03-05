#include "UniDataDevice.h"
#ifndef SMDDEVICE
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <mongocxx/instance.hpp>
using bsoncxx::builder::basic::kvp;
#endif
#include <boost/bind.hpp>
#include "common_func.h"
#include <boost/date_time/posix_time/posix_time.hpp>

template <typename CDataType, typename PClass, int RT_TYPE> UniDataDevice<CDataType, PClass, RT_TYPE>::UniDataDevice()
{
    memset(&cData, 0, sizeof(cData));
}

template <typename CDataType, typename PClass, int RT_TYPE> UniDataDevice<CDataType, PClass, RT_TYPE>::~UniDataDevice()
{
}

template <typename CDataType, typename PClass, int RT_TYPE>
bool UniDataDevice<CDataType, PClass, RT_TYPE>::InitSetting(const Json::Value& settingRoot)
{
    cData.data_id = this->data_id_;
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
		if(settingRoot["appSetting"]["b_mode"] != Json::nullValue) {
			b_mode_ = atof(settingRoot["appSetting"]["b_mode"].asString().c_str());
		}
	}    
    return PClass::InitSetting(settingRoot);
}

template <typename CDataType, typename PClass, int RT_TYPE>
bool UniDataDevice<CDataType, PClass, RT_TYPE>::LoadFromCache(const uint8_t* data, int data_len)
{
    if(data_len == sizeof(cData)) {
        memcpy(&cData, data, data_len);
        this->bIsDataReady_ = true;
        RoundDone();
        return true;
    }
    return false;
}

template <typename CDataType, typename PClass, int RT_TYPE> 
void UniDataDevice<CDataType, PClass, RT_TYPE>::DoCheck(std::vector<rule_value> rvVec)
{
    this->bSaveHistory |= this->DoBatchCheckThreshold(rvVec);
    //this->SaveDeviceHistory((unsigned char*)&cData, sizeof(cData));
}

template <typename CDataType, typename PClass, int RT_TYPE> void UniDataDevice<CDataType, PClass, RT_TYPE>::RoundDone()
{
    
    if(!this->bIsDataReady_)
        this->bIsDataReady_ = true;
    //std::cout<<"before runcheckthreshold 1"<<std::endl;
    this->lastTime = boost::posix_time::second_clock::local_time();
    cData.update_time = get_ttime(this->lastTime);
//    boost::gregorian::date d = this->lastTime.date();//ptime.date();
//    boost::posix_time::time_duration optd = this->lastTime.time_of_day();//ptime.time_of_day();
//
//    cData.update_time.year = d.year();
//    cData.update_time.month = d.month();
//    cData.update_time.day = d.day();
//    cData.update_time.hour = optd.hours();
//    cData.update_time.minute = optd.minutes();
//    cData.update_time.second = optd.seconds();
    //std::cout<<"before runcheckthreshold 2"<<std::endl;
    //return;
    this->SendSP(RT_TYPE, (unsigned char*)&cData, sizeof(cData));

    //std::cout<<"before runcheckthreshold 3"<<std::endl;
    //return;
    this->Reset();
    this->signal_index_ = 1;
    this->bSaveHistory = false;
    
            
    //std::cout<<"before runcheckthreshold"<<std::endl;
    RunCheckThreshold();
    //std::cout<<"after runcheckthreshold"<<std::endl;
    // 2022-10-18 这个地方可以对联通B接口的AO做统一处理
    for(auto aoSignal : this->aoVec_) {
        if(this->aoValueMap_.find(aoSignal) != this->aoValueMap_.end()) {
            this->bSaveHistory = this->CheckAOReport(aoSignal, this->aoValueMap_[aoSignal]) || this->bSaveHistory;
            this->CheckThreshold(aoSignal, "", this->aoValueMap_[aoSignal], 1);
        }
    }
    int rvSize = this->ruleValueVec.size();
    std::cout<<"rvSize is "<<rvSize<<std::endl;
    //auto rvVec(std::move(this->ruleValueVec));
    
    //this->CheckThresholdWork(boost::bind(&UniDataDevice<CDataType, PClass, RT_TYPE>::DoCheck, this, rvVec));
    //std::cout<<"before DoBatchCheckThreshold"<<std::endl;
    this->bSaveHistory |= this->DoBatchCheckThreshold(this->ruleValueVec);
    this->SaveDeviceHistory((unsigned char*)&cData, sizeof(cData));
    
    //  std::cout<<"after DoBatchCheckThreshold"<<std::endl;
    this->ruleValueVec.clear();
}
