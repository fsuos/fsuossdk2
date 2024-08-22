#ifndef DPJG1K_INTERFACE_H
#define DPJG1K_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct DPJG1K_Data_t {
    unsigned int data_id;
    
    uint16_t r3_10[14];

    tele_c_time update_time;
};

#pragma pack(pop)
#endif
