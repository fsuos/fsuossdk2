#ifndef {{ Project.Name }}_H
#define {{ Project.Name }}_H

#include "SMDDIDevice.hpp"
#include "{{ Project.Name }}_interface.h"
#include "UniDataDevice.h"

/**
 * @file {{ Project.File }}
 * @brief
 */
#define RT_{{ Project.Name }} {{ Project.RT_ID }}
class {{ Project.Name }}: public UniDataDevice<{{ Project.Name }}_Data_t, SMDDIDevice, RT_{{ Project.Name }}>
{
public:
    {{ Project.Name }}();
    ~{{ Project.Name }}();
    bool InitSetting(const Json::Value&  settingRoot);
    bool process(char di, bool first_time);
    void RunCheckThreshold() override;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    {% for s in InitSetting %}
    {% if s.Type == "int" %}
    int {{s.Name}}_;
    {% elif s.Type == "float" %}
    float {{s.Name}}_;
    {% endif %}
    {% endfor %}
};

PLUMA_INHERIT_PROVIDER({{ Project.Name }}, SMDDIDevice);
#endif // WATERSENSOR_H
