#ifndef Water3_INTERFACE_H
#define Water3_INTERFACE_H
#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)

struct Water3_Data_t {
        unsigned int data_id;
        uint8_t value;
        tele_c_time update_time; 
};      

#pragma pack(pop)
#endif  
