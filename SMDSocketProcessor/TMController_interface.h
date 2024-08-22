#ifndef TMController_INTERFACE_H
#define TMController_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct TMController_Data_t {
    unsigned int data_id;
    
    uint16_t r4_0[51];

    
    uint16_t r4_100[23];

    
    uint16_t r4_400[7];

    
    uint16_t r3_0[3];

    tele_c_time update_time;
};

#pragma pack(pop)
#endif
