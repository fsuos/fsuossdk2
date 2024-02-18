#ifndef HanGuang4PDC_INTERFACE_H
#define HanGuang4PDC_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct HanGuang4PDC_Data_t {
    unsigned int data_id;
    uint16_t r3_0[12];
    uint16_t r3_12[10];
    uint16_t r3_22[10];
    uint16_t r3_32[10];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
