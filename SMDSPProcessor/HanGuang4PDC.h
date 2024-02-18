#ifndef HanGuang4PDC_H
#define HanGuang4PDC_H
#include "SPModbus.h"
#include "HanGuang4PDC_interface.h"
#include "UniDataDevice.h"


/**
 * @file 4P直流4回路导轨表 说明书 20211216(2).pdf
 * @brief
 */
#define RT_HanGuang4PDC 5156
class HanGuang4PDC: public UniDataDevice<HanGuang4PDC_Data_t, SPModbus, RT_HanGuang4PDC>
{
public:
    HanGuang4PDC();
    ~HanGuang4PDC();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    enum HanGuang4PDC_Status {
        HanGuang4PDC_IDLE = 10,
	HanGuang4PDC_R3_0,
	HanGuang4PDC_R3_12,
	HanGuang4PDC_R3_22,
	HanGuang4PDC_R3_32,
        HanGuang4PDC_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(HanGuang4PDC, SMDSPDevice);
#endif
