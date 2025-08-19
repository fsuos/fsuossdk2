#ifndef SMDSOCKETDEVICE_H
#define SMDSOCKETDEVICE_H
#include "SMDVDevice.h"
#include <boost/thread.hpp>
#include "command.h"
#include <boost/asio/ip/tcp.hpp>
using boost::asio::ip::tcp;

#ifdef ENABLE_RCF
#include <RCF/Idl.hpp>

RCF_BEGIN(SMDSocketDeviceRPC, "SMDSocketDeviceRPC")
RCF_METHOD_R3(bool, Init, const std::string&, unsigned int, const std::string&);
RCF_METHOD_R0(bool, RefreshStatus);
RCF_METHOD_R2(bool, process, int, const std::string&);
RCF_METHOD_V1(void, OnConnected, int);
RCF_END(SMDSocketDeviceRPC);
#endif

class SMDSocketDevice : public SMDVDevice
{
public:
    SMDSocketDevice();
    ~SMDSocketDevice();

    virtual bool process(tcp::socket::native_handle_type fd, uint8_t *buffer, int size);

    virtual bool process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size) = 0;

    virtual void OnConnected(tcp::socket::native_handle_type fd);
    virtual bool RefreshStatus();

    virtual int StartSocket(const std::string& ip, uint16_t port) final;
    virtual int CloseSocket(int fd) final;

    virtual int SendSocketData(const unsigned char* data, size_t len) final
    {
        return SendSocketData(socket_, data, len);
    }
    virtual int SendSocketData(int fd, const unsigned char* data, size_t len) final;

    virtual int SendSocketData(const std::vector<boost::asio::const_buffer> &sendBuf) final
    {
        return SendSocketData(socket_, sendBuf);
    }
    virtual int SendSocketData(int fd, const std::vector<boost::asio::const_buffer> &sendBuf) final;

    virtual bool Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot);

protected:
    static const int socket_msgbuf_size = 4096;
    int msg_index;
    uint8_t msg_buf[socket_msgbuf_size];

    tcp::socket::native_handle_type socket_ = -1;

    std::string ip_;
    uint16_t port_ = 502;
    uint8_t addr_;//设备地址，好像都是1
    bool is_connected_ = false;

    int state;
};

PLUMA_PROVIDER_HEADER(SMDSocketDevice);

#endif // SMDSOCKETDEVICE_H
