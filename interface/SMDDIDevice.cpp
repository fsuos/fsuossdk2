#include "SMDDIDevice.hpp"

SMDDIDevice::SMDDIDevice()
{

}

bool SMDDIDevice::Init(std::weak_ptr<SMDController> controller,unsigned int  data_id, const Json::Value& settingRoot)
{
    SMDVDevice::Init(controller, data_id, settingRoot);
    //Load Last Value
    if(settingRoot["value"] != Json::nullValue) {
        value = settingRoot["value"].asBool();
    }
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
        if(settingRoot["appSetting"]["count"] != Json::nullValue) {
            hit_count = atof(settingRoot["appSetting"]["count"].asString().c_str());
        }
    }
    return  InitSetting(settingRoot);
}
PLUMA_PROVIDER_SOURCE(SMDDIDevice, 1, 1);
