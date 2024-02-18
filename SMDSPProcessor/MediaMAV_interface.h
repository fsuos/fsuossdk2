#ifndef MediaMAV_INTERFACE_H
#define MediaMAV_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct MediaMAV_Data_t {
    unsigned int data_id;
    uint8_t b1_0[114];
    uint16_t r3_0[61];
    uint16_t r3_62[6];
    uint16_t r3_168[1];
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
