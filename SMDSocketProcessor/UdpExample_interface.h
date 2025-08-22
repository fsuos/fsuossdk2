#ifndef UDPEXAMPLE_INTERFACE_H
#define UDPEXAMPLE_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)

struct UdpExample_Data_t{
    uint32_t data_id;    
    int    my_data;
    tele_c_time update_time;
};

#pragma pack(pop)
#endif

