#ifndef bzc_H
#define bzc_H
#include "SPModbus.h"
#include "bzc_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_bzc 5156
class bzc: public UniDataDevice<bzc_Data_t, SPModbus, RT_bzc>
{
public:
    bzc();
    ~bzc();
    bool InitSetting(const Json::Value&  settingRoot);
   bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
	void CallVoltage();
    void CallGroupVoltage();
	unsigned char GetMMCheckSum(unsigned char*data, int length)
    {
        unsigned char checksum = 0;
        for(int i = 0; i < length; i++) {
            checksum += data[i];
        }
        return checksum % 0xfe;
    }
    int ct_;
    int has_a_;
    enum bzc_Status {
        bzc_IDLE = 10,
	bzc_R3_7,
	bzc_R3_366,
	bzc_VOLTAGE, 
	bzc1_VOLTAGE,
        bzc_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(bzc, SMDSPDevice);
#endif
