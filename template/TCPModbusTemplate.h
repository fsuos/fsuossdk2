#ifndef {{ Project.Name }}_H
#define {{ Project.Name }}_H
#include "SMDSocketDevice.hpp"
#include "{{ Project.Name }}_interface.h"
#include "UniDataDevice.h"
#include <modbus/modbus.h>
#include <modbus/modbus-tcp.h>

/**
 * @file {{ Project.File }}
 * @brief
 */
#define RT_{{ Project.Name }} {{ Project.RT_ID }}
class {{ Project.Name }}: public UniDataDevice<{{ Project.Name }}_Data_t, SMDSocketDevice, RT_{{ Project.Name }}>
{
public:
    {{ Project.Name }}();
    ~{{ Project.Name }}();
    bool Init(std::weak_ptr<SMDController> controller,unsigned int  data_id, const Json::Value& settingRoot) override;
    void RunCheckThreshold() override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
    bool process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size);
private:
    {% if InitSetting is defined %}
    {% for s in InitSetting %}
    {% if s.Type == "int" %}
    int {{s.Name}}_;
    {% elif s.Type == "float" %}
    float {{s.Name}}_;
    {% endif %}
    {% endfor %}
    {% endif %}
    modbus_t* pCtx = nullptr;
    int cmd_result_ = -1;
    uint8_t last_data_ = -1;
};

PLUMA_INHERIT_PROVIDER({{ Project.Name }}, SMDSocketDevice);
#endif
