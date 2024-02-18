#ifndef AGC150_H
#define AGC150_H
#include "SPModbus.h"
#include "AGC150_interface.h"
#include "UniDataDevice.h"


/**
 * @file agc-150-modbus-tables-4189341212-中文.xlsx
 * @brief
 */
#define RT_AGC150 5156
class AGC150: public UniDataDevice<AGC150_Data_t, SPModbus, RT_AGC150>
{
public:
    AGC150();
    ~AGC150();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    enum AGC150_Status {
        AGC150_IDLE = 10,
	AGC150_R4_501,
	AGC150_R4_538,
	AGC150_R4_1018,
        AGC150_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(AGC150, SMDSPDevice);
#endif
