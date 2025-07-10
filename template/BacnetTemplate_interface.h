#ifndef {{ Project.Name }}_INTERFACE_H
#define {{ Project.Name }}_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)

struct {{ Project.Name }}_Data_t{
    uint32_t data_id;    
    {% for sc in Sample %}  
    {% if sc.Cmd == 3 or sc.Cmd == 4 or sc.Cmd == 5 %}
    uint8_t v{{ sc.Cmd }}[{{ sc.Offset|length }}];
    {% elif sc.Cmd == 0 or sc.Cmd == 1 or sc.Cmd == 2 %}
    float v{{ sc.Cmd }}[{{ sc.Offset|length }}];
    {% elif sc.Cmd == 19 %}
    uint32_t v{{ sc.Cmd }}[{{ sc.Offset|length }}];
    {% endif %}
    {% endfor %}
    tele_c_time update_time;
};

#pragma pack(pop)


#endif

