#ifndef {{ Project.Name }}_INTERFACE_H
#define {{ Project.Name }}_INTERFACE_H
#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)

struct {{ Project.Name }}_Data_t {
        unsigned int data_id;
        uint8_t value;
        tele_c_time update_time; 
};      

#pragma pack(pop)
#endif  
