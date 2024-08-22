#ifndef DTSD3366DJ_INTERFACE_H
#define DTSD3366DJ_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct DTSD3366DJ_Data_t {
    unsigned int data_id;
    uint32_t r3_0x8000[48];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
