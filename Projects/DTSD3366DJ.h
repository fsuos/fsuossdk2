#ifndef DTSD3366DJ_H
#define DTSD3366DJ_H
#include "SPModbus.h"
#include "DTSD3366DJ_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_DTSD3366DJ 5156
class DTSD3366DJ: public UniDataDevice<DTSD3366DJ_Data_t, SPModbus, RT_DTSD3366DJ>
{
public:
    DTSD3366DJ();
    ~DTSD3366DJ();
    
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
private:
    enum DTSD3366DJ_Status {
        DTSD3366DJ_IDLE = 10,
	DTSD3366DJ_R3_0x8000,
        DTSD3366DJ_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(DTSD3366DJ, SMDSPDevice);
#endif
