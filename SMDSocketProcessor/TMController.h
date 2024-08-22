#ifndef TMController_H
#define TMController_H
#include "SMDSocketDevice.hpp"
#include "TMController_interface.h"
#include "UniDataDevice.h"
#include <modbus/modbus.h>
#include <modbus/modbus-tcp.h>

/**
 * @file   
 * @brief
 */
#define RT_TMController 5156
class TMController: public UniDataDevice<TMController_Data_t, SMDSocketDevice, RT_TMController>
{
public:
    TMController();
    ~TMController();
    bool Init(std::weak_ptr<SMDController> controller,unsigned int  data_id, const Json::Value& settingRoot) override;
    void RunCheckThreshold() override;
    bool RefreshStatus() override;
    float Get_Value(uint32_t data_id, const std::string& var_name) const;
    int DeviceIoControl(int ioControlCode, void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
    bool process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size);
private:
    modbus_t* pCtx = nullptr;
    int cmd_result_ = -1;
    uint8_t last_data_ = -1;
};

PLUMA_INHERIT_PROVIDER(TMController, SMDSocketDevice);
#endif
