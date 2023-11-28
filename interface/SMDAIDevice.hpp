#ifndef SMDAIDEVICE_HPP
#define SMDAIDEVICE_HPP
#include "SMDVDevice.h"
#ifdef ENABLE_RCF
#include <RCF/Idl.hpp>

RCF_BEGIN(SMDAIDeviceRPC, "SMDAIDeviceRPC")
RCF_METHOD_R3(bool, Init, const std::string&, unsigned int, const std::string&);
RCF_METHOD_R1(float, process, short);
RCF_METHOD_R2(float, Get_Value, unsigned int, const std::string&);
RCF_END(SMDAIDeviceRPC);
#endif

class SMDAIDevice : public SMDVDevice
{
public:
    virtual float process(short ai) = 0;

    bool Init(std::weak_ptr<SMDController> controller, uint32_t data_id, const Json::Value& settingRoot) override;
protected:
    float value;
    SMDAIDevice();
    float a;
    float b;
private:
    using SMDVDevice::controller_;
#ifdef ENABLE_RCF
    using SMDVDevice::controller_rpc_;
    using SMDVDevice::p_io_service_;
#endif
};


PLUMA_PROVIDER_HEADER(SMDAIDevice);


#endif
