#ifndef Upsmodbus_INTERFACE_H
#define Upsmodbus_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct Upsmodbus_Data_t {
    unsigned int data_id;
    uint16_t r3_0[105];
    uint16_t r3_106[106];
	uint16_t alarm1,alarm2,alarm3,alarm4,alarm5,alarm6,alarm7,alarm8,alarm9;//状态告警
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
