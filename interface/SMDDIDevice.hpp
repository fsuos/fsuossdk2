#ifndef SMDDIDEVICE_HPP
#define SMDDIDEVICE_HPP
#include "SMDVDevice.h"
#include "SMDController.hpp"
#include <json/json.h>

#ifdef ENABLE_RCF
#include <RCF/Idl.hpp>
RCF_BEGIN(SMDDIDeviceRPC, "SMDDIDeviceRPC")
RCF_METHOD_R3(bool, Init, const std::string&, unsigned int, const std::string&);
RCF_METHOD_R2(bool, process, char, bool);
RCF_METHOD_R2(float, Get_Value, unsigned int, const std::string&);
RCF_END(SMDDIDeviceRPC);
#endif
//setting
//alert block;saving interval;user define settings
class SMDDIDevice : public SMDVDevice
{
public:
    virtual bool process(char di, bool first_time = false) = 0;
    virtual bool Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot) override;
protected:
    SMDDIDevice();

    bool value = true;
    int hit_count = 0;
private:
    using SMDVDevice::controller_;
#ifdef ENABLE_RCF
    using SMDVDevice::controller_rpc_;
    using SMDVDevice::p_io_service_;
#endif
};


PLUMA_PROVIDER_HEADER(SMDDIDevice);

#endif
