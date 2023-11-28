#ifndef DI_INTERFACE_H
#define DI_INTERFACE_H
#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)

struct DI_Data_t {
	unsigned int data_id;
	uint8_t value;
	tele_c_time update_time;
};

#pragma pack(pop)
#endif
