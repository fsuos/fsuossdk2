#ifndef Sxyw_INTERFACE_H
#define Sxyw_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct Sxyw_Data_t {
    unsigned int data_id;
    float  r3_level[15];//水箱液位和油罐液位
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
