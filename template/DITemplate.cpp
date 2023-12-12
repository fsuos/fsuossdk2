#include "{{ Project.Name }}.h"
#include "UniDataDevice.cpp"

{{ Project.Name }}::{{ Project.Name }}()
{
    device_type_ = "{{ Project.Name|lower }}";
}

bool {{ Project.Name }}::InitSetting(const Json::Value &settingRoot)
{
        cData.data_id = data_id_;
        if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
                {% for s in InitSetting %}
                if(settingRoot["appSetting"]["{{ s.Name }}"] != Json::nullValue) {
                        {{s.Name}}_ = {% if s.Type == "int" %}atoi({%else%}atof({%endif%}settingRoot["appSetting"]["{{ s.Name }}"].asString().c_str());
                }
                {% endfor %}
        }
        return UniDataDevice<{{ Project.Name }}_Data_t, SMDDIDevice, RT_{{ Project.Name }}>::InitSetting(settingRoot);
}

bool {{ Project.Name }}::process(char di, bool first_time)
{
    //User Specifal Code
    cData.value = value;
    RoundDone();
    return value;
}

void {{ Project.Name }}::RunCheckThreshold()
{
    CheckThreshold("value", "", value);
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<{{ Project.Name }}Provider>());
    return std::move(providerVec);
}
#endif
