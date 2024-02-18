#ifndef ETPower_H
#define ETPower_H
#include "SPModbus.h"
#include "ETPower_interface.h"
#include "UniDataDevice.h"


/**
 * @file 数显表ET系列电流表（电压表）通讯协议V1.3（2017.07.05）.xls
 * @brief
 */
#define RT_ETPower 5156
class ETPower: public UniDataDevice<ETPower_Data_t, SPModbus, RT_ETPower>
{
public:
    ETPower();
    ~ETPower();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    int has_a_;
    int has_b_;
    int has_c_;
    enum ETPower_Status {
        ETPower_IDLE = 10,
	ETPower_R3_1280,
        ETPower_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(ETPower, SMDSPDevice);
#endif
