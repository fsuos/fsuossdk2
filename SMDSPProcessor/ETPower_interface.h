#ifndef ETPower_INTERFACE_H
#define ETPower_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct ETPower_Data_t {
    unsigned int data_id;
    uint16_t r3_1280[24];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
