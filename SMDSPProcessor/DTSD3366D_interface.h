#ifndef DTSD3366D_INTERFACE_H
#define DTSD3366D_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct DTSD3366D_Data_t {
    unsigned int data_id;
    uint16_t r3_9[2];
    uint16_t r3_366[12];
    uint16_t r3_378[24];
    uint16_t r3_402[8];
    uint16_t r3_4096[2];
    uint16_t r3_4144[2];
    uint16_t r3_4192[2];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
