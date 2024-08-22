#ifndef bzc_INTERFACE_H
#define bzc_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct bzc_Data_t {
    unsigned int data_id;
    uint16_t r3_7[4];
    uint16_t r3_366[44];
	int bzc_VOLTAGE;
	int bzc1_VOLTAGE;
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
