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

#include "common_func.h"
#include <boost/date_time/posix_time/posix_time.hpp>

template <typename CDataType, typename PClass, int RT_TYPE> UniDataDevice<CDataType, PClass, RT_TYPE>::UniDataDevice()
{
    memset(&cData, 0, sizeof(cData));
}

template <typename CDataType, typename PClass, int RT_TYPE>
bool UniDataDevice<CDataType, PClass, RT_TYPE>::InitSetting(const Json::Value& settingRoot)
{
    cData.data_id = this->data_id_;
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

template <typename CDataType, typename PClass, int RT_TYPE> void UniDataDevice<CDataType, PClass, RT_TYPE>::RoundDone()
{
    if(!this->bIsDataReady_)
        this->bIsDataReady_ = true;
    this->lastTime = boost::posix_time::second_clock::local_time();
    cData.update_time = get_ttime(boost::posix_time::second_clock::local_time());
    std::cout << "RoundDone" << cData.data_id << " end " <<this->lastTime<< std::endl;
    this->SendSP(RT_TYPE, (unsigned char*)&cData, sizeof(cData));
    this->Reset();
    this->signal_index_ = 1;
    this->bSaveHistory = false;
    RunCheckThreshold();
    // 2022-10-18 这个地方可以对联通B接口的AO做统一处理
    for(auto aoSignal : this->aoVec_) {
        if(this->aoValueMap_.find(aoSignal) != this->aoValueMap_.end()) {
            this->bSaveHistory = this->CheckAOReport(aoSignal, this->aoValueMap_[aoSignal]) || this->bSaveHistory;
            this->CheckThreshold(aoSignal, "", this->aoValueMap_[aoSignal], 1);
        }
    }
    int rvSize = this->ruleValueVec.size();
    this->CheckThresholdWork([self(this->shared_from_this()), this, rvVec(std::move(this->ruleValueVec))] {
        this->bSaveHistory |= this->DoBatchCheckThreshold(rvVec);
        this->SaveDeviceHistory((unsigned char*)&cData, sizeof(cData));
    });
    this->ruleValueVec.reserve(rvSize);
}