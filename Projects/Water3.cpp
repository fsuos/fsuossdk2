#include "Water3.h"
#include "UniDataDevice.cpp"

Water3::Water3()
{
    device_type_ = "water3";
}

bool Water3::InitSetting(const Json::Value &settingRoot)
{
        cData.data_id = data_id_;
        if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
                if(settingRoot["appSetting"]["count"] != Json::nullValue) {
                        count_ = atoi(settingRoot["appSetting"]["count"].asString().c_str());
                }
        }
        return UniDataDevice<Water3_Data_t, SMDDIDevice, RT_Water3>::InitSetting(settingRoot);
}

bool Water3::process(char di, bool first_time)
{
    //User Specifal Code
    cData.value = value;
    RoundDone();
    return value;
}

void Water3::RunCheckThreshold()
{
    CheckThreshold("value", "", value);
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<Water3Provider>());
    return std::move(providerVec);
}
#endif
