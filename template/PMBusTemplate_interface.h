#ifndef {{ Project.Name }}_INTERFACE_H
#define {{ Project.Name }}_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct {{ Project.Name }}_Data_t {
    unsigned int data_id;
    {% for tsc in Sample %}
    uint8_t {{  "r%d_%d[%d];"%(tsc.CID1,tsc.CID2,tsc.Len) }}
    {% endfor %}
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
