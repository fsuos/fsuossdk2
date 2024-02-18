#ifndef DTSD3366D_H
#define DTSD3366D_H
#include "SPModbus.h"
#include "DTSD3366D_interface.h"
#include "UniDataDevice.h"


/**
 * @file DTSD3366D-W1 modbus通讯 协议V1.06.pdf
 * @brief
 */
#define RT_DTSD3366D 5156
class DTSD3366D: public UniDataDevice<DTSD3366D_Data_t, SPModbus, RT_DTSD3366D>
{
public:
    DTSD3366D();
    ~DTSD3366D();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    int ct_;
    int has_a_;
    enum DTSD3366D_Status {
        DTSD3366D_IDLE = 10,
	DTSD3366D_R3_9,
	DTSD3366D_R3_366,
	DTSD3366D_R3_378,
	DTSD3366D_R3_402,
	DTSD3366D_R3_4096,
	DTSD3366D_R3_4144,
	DTSD3366D_R3_4192,
        DTSD3366D_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(DTSD3366D, SMDSPDevice);
#endif
