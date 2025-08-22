#ifndef SMDUDPDEVICE_HPP
#define SMDUDPDEVICE_HPP
#include "SMDSocketDevice.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "boost/asio/deadline_timer.hpp"
#include <boost/thread/mutex.hpp>

using boost::asio::ip::udp;
class SMDUdpDevice : public SMDSocketDevice
{
public:
    SMDUdpDevice();
    ~SMDUdpDevice();
    
    bool Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot);

protected:
    int Send(uint8_t *data, int data_len);
    int Receive();
    void DeadlineHandler(boost::system::error_code ec) ;
    void handle_receive(const boost::system::error_code& error,
                                  std::size_t bytes_transferred);
                                  
    //udp socket
    boost::asio::io_service io_context_;
    
    boost::mutex socket_mutex_;
    udp::socket socket_;
	
	udp::endpoint remote_endpoint_;
    udp::endpoint peer_endpoint_;
    
    uint8_t npdu[1506] = { 0 };
   
    boost::asio::deadline_timer deadline_;
    bool timed_out_;
};

#endif // SMDUDPDEVICE_HPP
