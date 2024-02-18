#ifndef EvoCyberMateDXModbus_H
#define EvoCyberMateDXModbus_H
#include "SPModbus.h"
#include "EvoCyberMateDXModbus_interface.h"
#include "UniDataDevice.h"


/**
 * @file 05130040-通信协议-A7-EVO-CyberMate-DX-modbus协议-中英文-2021-11-29  英维克.pdf
 * @brief
 */
#define RT_EvoCyberMateDXModbus 5156
class EvoCyberMateDXModbus: public UniDataDevice<EvoCyberMateDXModbus_Data_t, SPModbus, RT_EvoCyberMateDXModbus>
{
public:
    EvoCyberMateDXModbus();
    ~EvoCyberMateDXModbus();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    enum EvoCyberMateDXModbus_Status {
        EvoCyberMateDXModbus_IDLE = 10,
	EvoCyberMateDXModbus_R3_256,
        EvoCyberMateDXModbus_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(EvoCyberMateDXModbus, SMDSPDevice);
#endif
