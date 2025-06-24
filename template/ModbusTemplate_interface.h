#ifndef {{ Project.Name }}_INTERFACE_H
#define {{ Project.Name }}_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct {{ Project.Name }}_Data_t {
    unsigned int data_id;
    {% for tsc in Sample %}
    {% if tsc.CmdGroupStart is defined %}
        struct {
        {% for sc in tsc.CmdGroupSample %}
            {% if sc.Cmd == 3 or sc.Cmd == 4 %}
            uint16_t {{  "r%d_%d_%d[%d];"%(sc.Cmd, tsc.CmdGroupStart, sc.Offset, sc.Len) }}
            {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
            uint8_t {{  "b%d_%d_%d[%d];"%(sc.Cmd, tsc.CmdGroupStart, sc.Offset, sc.Len) }}
            {% endif %}
        {% endfor %}
        }{{  "r_%d[%d];"%(tsc.CmdGroupStart,(tsc.CmdGroupEnd-tsc.CmdGroupStart)/tsc.CmdGroupStep) }}
    {% else %}
    
    {% if tsc.Cmd == 3 or tsc.Cmd == 4 %}
    uint16_t {{  "r%d_%d[%d];"%(tsc.Cmd,tsc.Offset,tsc.Len) }}
    {% elif tsc.Cmd == 1 or tsc.Cmd == 2 %}
    uint8_t {{  "b%d_%d[%d];"%(tsc.Cmd,tsc.Offset,tsc.Len) }}
    {% endif %}

    {% endif %}
    {% endfor %}
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
