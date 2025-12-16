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
    b_mode_ = 1;
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
			b_mode_ = atoi(settingRoot["appSetting"]["b_mode"].asString().c_str());
            std::cout<<"InitSetting "<<cData.data_id<<" b_mode_:"<<b_mode_<<std::endl;
		}
	}
    if(3 == b_mode_){
        //移动需要解析额外的配置
        std::cout<<"here 1"<<std::endl;
        if(settingRoot["thresholdSetting"] != Json::nullValue) {
            std::cout<<"here 2"<<std::endl;
            for(auto it = settingRoot["thresholdSetting"].begin(); it != settingRoot["thresholdSetting"].end(); it++) {
                const std::string& signalId = it.key().asString();
                std::cout<<"here "<<signalId<<std::endl;
                const Json::Value& setting = settingRoot["thresholdSetting"][signalId];
                if(setting.type() == Json::objectValue) {
                    if(setting["SignalName"] != Json::nullValue &&
                        setting["SignalName"].type() != Json::nullValue) { // 读电信配置方案告警等级
                        // boost::trim(sAlarmLevel);
                        signalNameMap_[signalId] = setting["SignalName"].asString();
                    } 
                    //读移动配置方案告警等级
                    std::string mobileExtra;//619-007-00-007002;007002;000
                    if(setting["NMAlarmID"] != Json::nullValue &&
                        setting["NMAlarmID"].type() != Json::nullValue) { // 读移动配置方案告警阀值
                        mobileExtra += setting["NMAlarmID"].asString();
                    }else{
                        std::cout<<this->data_id_<<" NMAlarmID is missing"<<std::endl;
                    }
                    mobileExtra += ";";
                    if(setting["ID"] != Json::nullValue &&
                        setting["ID"].type() != Json::nullValue) { // 读移动配置方案告警阀值
                        mobileExtra += setting["ID"].asString();
                    }else{
                        std::cout<<this->data_id_<<" ID is missing"<<std::endl;
                    }
                    mobileExtra += ";";
                    if(setting["SignalNumber"] != Json::nullValue &&
                        setting["SignalNumber"].type() != Json::nullValue) { // 读电信配置方案告警阀值
                        mobileExtra += setting["SignalNumber"].asString();
                    }else{
                        std::cout<<this->data_id_<<" SignalNumber is missing"<<std::endl;
                    }
                    mobileExtra += ";";
                    std::cout<<"mobile:"<<signalId<<":"<<mobileExtra<<" level:"<<this->levelMap_[signalId]<<" value:"<<this->valueMap_[signalId]<<std::endl;
                    singalExtraMap_[signalId] = mobileExtra;
                }
            }
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

template <typename CDataType, typename PClass, int RT_TYPE>
bool UniDataDevice<CDataType, PClass, RT_TYPE>::CheckAOReport(const std::string& aoSignalId, const std::string& reportSignalId, float value)
{
    bool bChanged = false;
    if(this->aoMap.find(reportSignalId) == this->aoMap.end()){
	if(this->aoMap.find(aoSignalId) == this->aoMap.end()){
	    return false;
	}
	this->aoMap[reportSignalId] = this->aoMap[aoSignalId];
    }
    SMDVDevice::BAOSetting& aoSetting = this->aoMap[reportSignalId];
    //std::cout << "CheckAOReport signalId:" << aoSignalId << "last value:" << aoSetting.lastValue << " value:" << value<< std::endl;
    if(aoSetting.Threshold_valid) {
        //std::cout << "Threshold:" << aoSetting.Threshold << std::endl;
        if(value > (aoSetting.lastValue + aoSetting.Threshold) || value < (aoSetting.lastValue - aoSetting.Threshold)) {
            bChanged = true;
            //std::cout << "hit" << std::endl;
        }
    }
    if(aoSetting.RelativeVal_valid) {
        //std::cout << "Threshold:" << aoSetting.RelativeVal << std::endl;
        if(value > (aoSetting.lastValue * (100 + aoSetting.RelativeVal) / 100) ||
            value < (aoSetting.lastValue * (100 - aoSetting.RelativeVal) / 100)) {
            bChanged = true;
            //std::cout << "hit" << std::endl;
        }
    }
    if(aoSetting.IntervalTime_valid) {
        //std::cout << "Threshold:" << aoSetting.IntervalTime << std::endl;
        if(time(NULL) > (aoSetting.lastUploadSecond + 60 * aoSetting.IntervalTime)) {
            bChanged = true;
            //std::cout << "hit" << std::endl;
        }
    }

    if(bChanged) {
        //std::cout << "CheckAOReport signalId:" << aoSignalId << " value:" << value << std::endl;
        aoSetting.lastValue = value;
        aoSetting.lastUploadSecond = time(NULL);
        // 调用SMDVDevice
	SMDVDevice::CheckAOReport(reportSignalId, value);
        return true;
    }
    return false;
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
    // 2025-11-1 看起来没法统一处理了
    /*for(auto aoSignal : this->aoVec_) {
        if(this->aoValueMap_.find(aoSignal) != this->aoValueMap_.end()) {
            this->bSaveHistory = this->CheckAOReport(aoSignal, this->aoValueMap_[aoSignal]) || this->bSaveHistory;
            this->CheckThreshold(aoSignal, "", this->aoValueMap_[aoSignal], 1);
        }
    }*/
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
