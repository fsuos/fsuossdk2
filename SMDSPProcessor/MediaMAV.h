#ifndef MediaMAV_H
#define MediaMAV_H
#include "SPModbus.h"
#include "MediaMAV_interface.h"
#include "UniDataDevice.h"


/**
 * @file 美的精密机房用空调机组Modbus协议-V4.3
 * @brief
 */
#define RT_MediaMAV 5157
class MediaMAV: public UniDataDevice<MediaMAV_Data_t, SPModbus, RT_MediaMAV>
{
public:
    MediaMAV();
    ~MediaMAV();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    enum MediaMAV_Status {
        MediaMAV_IDLE = 10,
	MediaMAV_R1_0,
	MediaMAV_R3_0,
	MediaMAV_R3_62,
	MediaMAV_R3_168,
        MediaMAV_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(MediaMAV, SMDSPDevice);
#endif
