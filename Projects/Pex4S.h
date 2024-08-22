#ifndef Pex4S_H
#define Pex4S_H
#include "SPModbus.h"
#include "Pex4S_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_Pex4S 5156
class Pex4S: public UniDataDevice<Pex4S_Data_t, SPModbus, RT_Pex4S>
{
public:
    Pex4S();
    ~Pex4S();
    bool InitSetting(const Json::Value&  settingRoot);
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    int ct_;
    int has_a_pex_;
    enum Pex4S_Status {
        Pex4S_IDLE = 10,
	Pex4S_R3_20,
	Pex4S_R3_21,
        Pex4S_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(Pex4S, SMDSPDevice);
#endif
