#ifndef {{ Project.Name }}_H
#define {{ Project.Name }}_H
#include "SPModbus.h"
#include "{{ Project.Name }}_interface.h"
#include "UniDataDevice.h"


/**
 * @file {{ Project.File }}
 * @brief
 */
#define RT_{{ Project.Name }} {{ Project.RT_ID }}
class {{ Project.Name }}: public UniDataDevice<{{ Project.Name }}_Data_t, SPModbus, RT_{{ Project.Name }}>
{
public:
    {{ Project.Name }}();
    ~{{ Project.Name }}();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    {% for s in InitSetting %}
    {% if s.Type == "int" %}
    int {{s.Name}}_;
    {% elif s.Type == "float" %}
    float {{s.Name}}_;
    {% endif %}
    {% endfor %}
    enum {{ Project.Name }}_Status {
        {{ Project.Name }}_IDLE = 10,
	{% for sc in Sample %}
	{{ Project.Name + "_R%d_%d"%(sc.Cmd,sc.Offset) + "," }}
	{% endfor %}
        {{ Project.Name }}_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER({{ Project.Name }}, SMDSPDevice);
#endif
