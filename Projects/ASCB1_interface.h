#ifndef ASCB1_INTERFACE_H
#define ASCB1_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct ASCB1_Data_t {
    unsigned int data_id;
    uint16_t r3_0x000A[16];
    float   r3_0x001A[57];
	uint16_t alarm1,alarm2,alarm3,alarm4;//告警1～4
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
