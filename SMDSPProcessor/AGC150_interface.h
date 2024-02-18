#ifndef AGC150_INTERFACE_H
#define AGC150_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct AGC150_Data_t {
    unsigned int data_id;
    uint16_t r4_501[27];
    uint16_t r4_538[13];
    uint16_t r4_1018[3];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
