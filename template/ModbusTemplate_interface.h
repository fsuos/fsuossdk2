#ifndef {{ Project.Name }}_INTERFACE_H
#define {{ Project.Name }}_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct {{ Project.Name }}_Data_t {
    unsigned int data_id;
    {% for sc in Sample %}
    {% if sc.Cmd == 3 or sc.Cmd == 4 %}
    uint16_t {{  "r%d_%d[%d];"%(sc.Cmd,sc.Offset,sc.Len) }}
    {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
    uint8_t {{  "b%d_%d[%d];"%(sc.Cmd,sc.Offset,sc.Len) }}
    {% endif %}
    {% endfor %}
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
