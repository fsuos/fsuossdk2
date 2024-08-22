#ifndef SOCKETAGC150_HPP
#define SOCKETAGC150_HPP
#include "SMDSocketDevice.hpp"
#include "SocketAGC150_interface.h"
#include "UniDataDevice.h"
#include <modbus/modbus.h>
#include <modbus/modbus-tcp.h>

#define RT_SOCKETAGC150 5010
/**
 * @class AGC150
 * @author marship
 * @brief FSU:121.12.50.19   油机:121.12.50.59
 */
class SocketAGC150: public UniDataDevice<AGC150_Data_t,SMDSocketDevice, RT_SOCKETAGC150>
{
public:
        SocketAGC150();
        ~SocketAGC150();
        bool Init(std::weak_ptr<SMDController> controller,unsigned int  data_id, const Json::Value& settingRoot) override;
        bool RefreshStatus() override;
		void RunCheckThreshold() override;
        bool process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size);
private:
        modbus_t* pCtx = nullptr;
};


PLUMA_INHERIT_PROVIDER(SocketAGC150, SMDSocketDevice);
#endif