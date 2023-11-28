#ifndef SMDSPDEVICE_HPP
#define SMDSPDEVICE_HPP
#include <string>
#include "SMDController.hpp"
#include "SMDVDevice.h"
#include <boost/thread.hpp>
#include "command.h"

#ifdef ENABLE_RCF
#include <RCF/Idl.hpp>

RCF_BEGIN(SMDSPDeviceRPC, "SMDSPDeviceRPC")
RCF_METHOD_R3(bool, Init, const std::string&, unsigned int, const std::string&);
RCF_METHOD_R0(bool, RefreshStatus);
RCF_METHOD_R0(bool, Reset);
RCF_METHOD_R1(bool, process, const std::string&);
RCF_METHOD_R0(bool, IsProcessing);
RCF_METHOD_R0(bool, IsStalled);
RCF_METHOD_R2(float, Get_Value, unsigned int, const std::string&);
RCF_END(SMDSPDeviceRPC);
#endif

class SMDSPDevice: public SMDVDevice
{
public:
    SMDSPDevice();

    virtual bool Init(std::weak_ptr<SMDController> controller, unsigned int data_id, const Json::Value& settingRoot) override;

    virtual bool RefreshStatus();

    virtual bool Reset();
    //If process return true, we still need to use this sp link, if false, scheduler should try next
    virtual bool process(unsigned char* data, size_t len);

    //是否正在处理，返回true，底层的数据会进入本协议
    virtual bool IsProcessing();
    //是否卡住了，判断标准一般是协议发出去，但是超时时间内没有收到
    virtual bool IsStalled();

protected:
    virtual bool SendData(const unsigned char *data, size_t len) final;
    virtual bool SendParityData(const unsigned char *data, size_t len, EnumParity type) final;

    enum {IDLE = 0, RUN, ERROR};

    int state;



    uint8_t addr_;//485 addr
    //这个msg_buf准备放到下面去，以节省内存,主要是不同设备对内存缓存的需求不一样，电池最大的协议才67个长度
    unsigned char msg_buf[4096];

    unsigned int msg_index;

    boost::mutex mutex;

    //用来控制通道的状态
    boost::shared_mutex rwMutex;
    int channel_state;

    bool isStalled_;

    bool bNoAutoOpen_;

    virtual bool OpenPort() final;
    virtual bool ClosePort() final;
    int baud_rate_ = 9600; /// 波特率
    int char_size_ = 8;
    int stop_bit_size_ = 1;
    EnumParity parity_ = NONE;

    bool bLastSaveStatus = false;
private:
    using SMDVDevice::controller_;
#ifdef ENABLE_RCF
    using SMDVDevice::controller_rpc_;
    using SMDVDevice::p_io_service_;
#endif
};


PLUMA_PROVIDER_HEADER(SMDSPDevice);


#endif
