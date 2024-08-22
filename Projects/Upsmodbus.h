#ifndef Upsmodbus_H
#define Upsmodbus_H
#include "SPModbus.h"
#include "Upsmodbus_interface.h"
#include "UniDataDevice.h"


/**
 * @file 测试.pdf
 * @brief
 */
#define RT_Upsmodbus 5156
class Upsmodbus: public UniDataDevice<Upsmodbus_Data_t, SPModbus, RT_Upsmodbus>
{
public:
    Upsmodbus();
    ~Upsmodbus();
   
    void RunCheckThreshold() override;
    bool process_payload(enum tab_type type, size_t len) override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
   
private:
    
    enum Upsmodbus_Status {
        Upsmodbus_IDLE = 10,
	Upsmodbus_R3_0,
	Upsmodbus_R3_106,
        Upsmodbus_END
    };
    int cmd_result_ = -1;
};

PLUMA_INHERIT_PROVIDER(Upsmodbus, SMDSPDevice);
#endif
