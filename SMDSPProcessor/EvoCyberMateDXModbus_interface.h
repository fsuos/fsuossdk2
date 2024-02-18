#ifndef EvoCyberMateDXModbus_INTERFACE_H
#define EvoCyberMateDXModbus_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct EvoCyberMateDXModbus_Data_t {
    unsigned int data_id;
    uint16_t r3_256[40];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
