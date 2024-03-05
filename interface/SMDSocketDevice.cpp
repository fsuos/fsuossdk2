#include "SMDSocketDevice.hpp"

SMDSocketDevice::SMDSocketDevice(): msg_index(0)
{

}

SMDSocketDevice::~SMDSocketDevice()
{
    if(-1 != socket_) {
        CloseSocket(socket_);
    }
}


int SMDSocketDevice::StartSocket(const std::string& ip, uint16_t port)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->StartSocket(data_id_, ip, port);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        try {
            return controller_rpc_->StartSocket(data_id_, ip, port);
        } catch(const RCF::Exception &e) {
            //std::cout<<"StartSocket "<<e.getErrorMessage()<<std::endl;
        }
    }
#endif
    return -1;
}

int SMDSocketDevice::CloseSocket(int fd)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->CloseSocket(data_id_,  fd);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        try {
            return controller_rpc_->CloseSocket(data_id_,  fd);
        } catch(const RCF::Exception &e) {
            //std::cout<<"StartSocket "<<e.getErrorMessage()<<std::endl;
        }
    }
#endif
    return -1;
}

int SMDSocketDevice::SendSocketData(int fd, const unsigned char* data, size_t len)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SendSocketData(data_id_,  fd, data, len);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        std::string sData((const char*)data, len);
        try {
            return controller_rpc_->SendSocketData(data_id_,  fd, sData);
        } catch(const RCF::Exception &e) {
            //std::cout<<"StartSocket "<<e.getErrorMessage()<<std::endl;
        }
    }
#endif
    return -1;
}

int SMDSocketDevice::SendSocketData(int fd, const std::vector<boost::asio::const_buffer> &sendBuf)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SendSocketData(data_id_,  fd, sendBuf);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        std::string sData;
        for(const auto& buf : sendBuf) {
            sData.append(boost::asio::buffer_cast<const char*>(buf), boost::asio::buffer_size(buf));
        }
        try {
            return controller_rpc_->SendSocketData(data_id_,  fd, sData);
        } catch(const RCF::Exception &e) {
            //std::cout<<"StartSocket "<<e.getErrorMessage()<<std::endl;
        }
    }
#endif
    return -1;
}


bool SMDSocketDevice::Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot)
{
    SMDVDevice::Init(controller, data_id, settingRoot);
    //app settings
    if(settingRoot["appSetting"] != Json::nullValue) {
        if(settingRoot["appSetting"].type() == Json::objectValue) {
            if(settingRoot["appSetting"]["ip"] != Json::nullValue) {
                ip_ = settingRoot["appSetting"]["ip"].asString();
            }
            if(settingRoot["appSetting"]["port"] != Json::nullValue) {
                port_ = atoi(settingRoot["appSetting"]["port"].asString().c_str());
            }
            if(settingRoot["appSetting"]["addr"] != Json::nullValue) {
                addr_ = atoi(settingRoot["appSetting"]["addr"].asString().c_str());
            }
        }
    }
    return true;
}

void SMDSocketDevice::OnConnected(tcp::socket::native_handle_type fd)
{
    is_connected_ = true;
}

bool SMDSocketDevice::process(tcp::socket::native_handle_type fd, uint8_t *data, int len)
{
    if(len == -1) {
        is_connected_ = false;
        if(-1 != socket_) {
            CloseSocket(socket_);
            socket_ = -1;
        }
        //socket disconnect
        return false;
    }
    return process_data(fd, data, len);
}

bool SMDSocketDevice::RefreshStatus()
{
    if(!is_connected_) {
        if(-1 != socket_) {
            CloseSocket(socket_);
        }
        socket_ = StartSocket(ip_, port_);
        return false;
    }
    return true;
}

PLUMA_PROVIDER_SOURCE(SMDSocketDevice, 1, 1);
