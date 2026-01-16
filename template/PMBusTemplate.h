#ifndef {{ Project.Name }}_H
#define {{ Project.Name }}_H
#include "PMBusProtocol.h"
#include "{{ Project.Name }}_interface.h"
#include "UniDataDevice.h"


/**
 * @file {{ Project.File }}
 * @brief
 */
#define RT_{{ Project.Name }} {{ Project.RT_ID }}
class {{ Project.Name }}: public UniDataDevice<{{ Project.Name }}_Data_t, PMBusProtocol, RT_{{ Project.Name }}>
{
public:
    {{ Project.Name }}();
    ~{{ Project.Name }}();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload() override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    {% if InitSetting is defined %}
    {% for s in InitSetting %}
    {% if s.Type == "int" %}
    int {{s.Name}}_ = {% if s.Default is defined %}{{s.Default}}{% else %}0{% endif %};
    {% elif s.Type == "uint32_t" %}
    uint32_t {{s.Name}}_ = {% if s.Default is defined %}{{s.Default}}{% else %}0{% endif %};
    {% elif s.Type == "float" %}
    float {{s.Name}}_ = {% if s.Default is defined %}{{s.Default}}{% else %}0.0{% endif %};
    {% endif %}
    {% endfor %}
    {% endif %}
    enum {{ Project.Name }}_Status {
        {{ Project.Name }}_IDLE = 10,
	{% for tsc in Sample %}
	{{ Project.Name + "_R%d_%d_%d"%(tsc.CID1,tsc.CID2,tsc.InfoLabel)+ "," }}
	{% endfor %}
        {{ Project.Name }}_END
    };
    int cmd_result_ = -1;
    int last_data_ = -1;
    int cmdgroup_step_ = 0;
};

PLUMA_INHERIT_PROVIDER({{ Project.Name }}, SMDSPDevice);
#endif
