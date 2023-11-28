#ifndef AI_INTERFACE_H
#define AI_INTERFACE_H
#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)

struct AI_Data_t {
	unsigned int data_id;
	float value;
	tele_c_time update_time;
};

#pragma pack(pop)
#endif
