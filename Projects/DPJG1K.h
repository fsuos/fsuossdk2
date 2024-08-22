#ifndef DPJG1K_H
#define DPJG1K_H
#include "SPModbus.h"
#include "DPJG1K_interface.h"
#include "UniDataDevice.h"


/**
 * @file DPJ-G1K 通信协议.pdf
 * @brief
 */
#define RT_DPJG1K 5156
class DPJG1K: public UniDataDevice<DPJG1K_Data_t, SPModbus, RT_DPJG1K>
{
public:
    DPJG1K();
    ~DPJG1K();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    int ct_;
    int has_a_;
    enum DPJG1K_Status {
        DPJG1K_IDLE = 10,
	DPJG1K_R3_10,
        DPJG1K_END
    };
    int cmd_result_ = -1;
    uint8_t last_data_ = -1;
    int cmdgroup_step_ = 0;
};

PLUMA_INHERIT_PROVIDER(DPJG1K, SMDSPDevice);
#endif
