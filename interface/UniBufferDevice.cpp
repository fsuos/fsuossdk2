#include "UniBufferDevice.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "common_func.h"




template<typename PClass, int RT_TYPE>
UniBufferDevice<PClass, RT_TYPE>::UniBufferDevice()
{
}

template<typename PClass, int RT_TYPE>
bool UniBufferDevice<PClass, RT_TYPE>::InitSetting(const Json::Value &settingRoot)
{
    return PClass::InitSetting(settingRoot);
}


template<typename PClass, int RT_TYPE>
bool UniBufferDevice<PClass, RT_TYPE>::LoadFromCache(const uint8_t *data, int data_len)
{   
     if(buffer_len_ != data_len){
        buffer_len_ = data_len;
        buffer_ = std::make_unique<uint8_t[]>(buffer_len_);
    }
    memcpy(buffer_.get(), data, data_len);
    this->bIsDataReady_ = true;
    this->lastTime = boost::posix_time::second_clock::local_time();
    return true;
}


template<typename PClass, int RT_TYPE>
void UniBufferDevice<PClass, RT_TYPE>::RoundDone()
{
    if(!this->bIsDataReady_)
        this->bIsDataReady_ = true;
	this->signal_index_= 1;
    this->bSaveHistory = false;
    RunCheckThreshold();
    //2022-10-18 这个地方可以对联通B接口的AO做统一处理
    for(auto aoSignal : this->aoVec_){
        if(this->aoValueMap_.find(aoSignal) != this->aoValueMap_.end()){
            this->bSaveHistory = this->CheckAOReport(aoSignal, this->aoValueMap_[aoSignal])|| this->bSaveHistory;
            this->CheckThreshold(aoSignal, "", this->aoValueMap_[aoSignal], 1);
        }
    }

    this->lastTime = boost::posix_time::second_clock::local_time();
    this->SendSP(RT_TYPE, buffer_.get(), buffer_len_);
    this->Reset();
    
    int rvSize = this->ruleValueVec.size();
    this->CheckThresholdWork([self(this->shared_from_this()), this, rvVec(std::move(this->ruleValueVec))] {
        this->bSaveHistory |= this->DoBatchCheckThreshold(rvVec);
        this->SaveDeviceHistory(buffer_.get(), buffer_len_);
    });
    this->ruleValueVec.reserve(rvSize);
}
