#ifndef Coolsmart_H
#define Coolsmart_H
#include "SPModbus.h"
#include "Coolsmart_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_Coolsmart 5156
class Coolsmart: public UniDataDevice<Coolsmart_Data_t, SPModbus, RT_Coolsmart>
{
public:
    Coolsmart();
    ~Coolsmart();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    int ct_;
    int has_a_;
    enum Coolsmart_Status {
        Coolsmart_IDLE = 10,
	Coolsmart_R3_7,
	Coolsmart_R3_366,
        Coolsmart_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(Coolsmart, SMDSPDevice);
#endif
