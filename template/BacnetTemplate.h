#ifndef {{ Project.Name }}_H
#define {{ Project.Name }}_H
#include "SMDBacNetDevice.hpp"
#include "{{ Project.Name }}_interface.h"
#include "UniDataDevice.h"


/**
 * @file {{ Project.File }}
 * @brief
 */
#define RT_{{ Project.Name }} {{ Project.RT_ID }}
class {{ Project.Name }}: public UniDataDevice<{{ Project.Name }}_Data_t,SMDBacNetDevice, RT_{{ Project.Name }}>
{
public:
        {{ Project.Name }}();
        ~{{ Project.Name }}();
        bool RefreshStatus() override;
        bool process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size);
	
};


PLUMA_INHERIT_PROVIDER({{ Project.Name }}, SMDSocketDevice);
#endif

