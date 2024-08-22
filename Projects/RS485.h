#ifndef RS485_H
#define RS485_H
#include "SMDSPDevice.hpp"
#include "UniDataDevice.h"
#include "UniBufferDevice.h"
#include "SPModbus.h"
#include "RS485_interface.h"

#define RT_RS485 5180
class RS485: public UniDataDevice<RS485_Data_t, SPModbus, RT_RS485>
{
public:
    RS485();
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string &var_name) const ;
protected:
    void CallVoltage();
    void CallGroupVoltage();

    enum {RS485_IDLE, RS485_VOLTAGE};
    int state;//query device state
//    int version_;
//    int dir_ = 0;//0 负极排序 1 正极排序
//    int iRetryCount;
//    float group_voltage;
//    float group_current;
//    int rs485_type;
//    
//    float group_a;
//    float group_b;
//    float rs485_adjust;
};

PLUMA_INHERIT_PROVIDER (RS485, SMDSPDevice );
#endif // BATTERY_H
