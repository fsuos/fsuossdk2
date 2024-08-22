#ifndef ASCB1_H
#define ASCB1_H
#include "SPModbus.h"
#include "ASCB1_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_ASCB1 5156
class ASCB1: public UniDataDevice<ASCB1_Data_t, SPModbus, RT_ASCB1>
{
public:
    ASCB1();
    ~ASCB1();
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
	int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned);
private:
    enum ASCB1_Status {
        ASCB1_IDLE = 10,
	ASCB1_R3_0x000A,
	ASCB1_R3_0x001A,
        ASCB1_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(ASCB1, SMDSPDevice);
#endif
