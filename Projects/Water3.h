#ifndef Water3_H
#define Water3_H

#include "SMDDIDevice.hpp"
#include "Water3_interface.h"
#include "UniDataDevice.h"

/**
 * @file 测试.pdf
 * @brief
 */
#define RT_Water3 5156
class Water3: public UniDataDevice<Water3_Data_t, SMDDIDevice, RT_Water3>
{
public:
    Water3();
    ~Water3();
    bool InitSetting(const Json::Value&  settingRoot);
    bool process(char di, bool first_time);
    void RunCheckThreshold() override;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
private:
    int count_;
};

PLUMA_INHERIT_PROVIDER(Water3, SMDDIDevice);
#endif // WATERSENSOR_H
