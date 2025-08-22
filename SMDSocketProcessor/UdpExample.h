#ifndef UDPEXAMPLE_HPP
#define UDPEXAMPLE_HPP
#include "SMDUdpDevice.hpp"
#include "UniDataDevice.h"
#include "UdpExample_interface.h"

#define RT_UdpExample 5010
/**
 * @brief
 */
class UdpExample: public UniDataDevice<UdpExample_Data_t,SMDUdpDevice, RT_UdpExample>
{
public:
        UdpExample();
        ~UdpExample();
        bool RefreshStatus() override;
        bool process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size);
        
        int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
        
private:
    int cmd_result_ = -1;
};


PLUMA_INHERIT_PROVIDER(UdpExample, SMDSocketDevice);
#endif

