#include "UdpExample.h"
#include "UniDataDevice.cpp"
#include <future>


UdpExample::UdpExample()
{
    device_type_ = "udpexample";
    addr_ = 1;
}

UdpExample::~UdpExample()
{
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
    memcpy(pFloat, buf+1, 2);
    memcpy(pFloat+2, buf, 2);
}


bool UdpExample::RefreshStatus()
{
    //如果想并发
    //std::async( std::launch::async,
    //                    [&]() {
        //同步操作不需要状态机
        {
            //如果有控制，每组收发，上下锁，这样可以保证控制代码快速执行
            boost::mutex::scoped_lock spLock(socket_mutex_);
            uint8_t cmd[3] = { 1, 2, 3};
            if(0 == Send(cmd, sizeof(cmd)))
            {
                if(0 == Receive())
                {
                    //msg_buf和msg_index配合
                    cData.my_data = msg_buf[0];
                    RoundDone();
                    return false;
                }
            }
        }
    //});
    return false;
}

bool UdpExample::process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size)
{
    //UDP 没有做成异步模式，做成了同步模式
        return false;
}



int UdpExample::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
{
    switch(ioControlCode) {
        case 310:{
            //SET_AODATA
            std::string aoStr((char*)inBuffer, inBufferSize);
            Json::Value setting;//
            Json::Reader reader;//解析
            if(!reader.parse(aoStr, setting)) {
              return -1;
            }
            cmd_result_ = 0;
            if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
                if(setting["signal_id"].asString() == "123455") {
                    int newValue = atoi(setting["SetValue"].asString().c_str());
                    //如果有控制，每组收发，上下锁，这样可以保证控制代码快速执行
                    boost::mutex::scoped_lock spLock(socket_mutex_);
                    uint8_t cmd[3] = { 1, 2, newValue};
                    if(0 == Send(cmd, sizeof(cmd)))
                    {
                        if(0 == Receive())
                        {
                            //msg_buf和msg_index配合
                            cmd_result_ = 1;
                            return 0;
                        }
                    }
                
                }
            }
            break;
        }
       case 320: {        
            state = 320;
            //320是SET_DO
            std::string aoStr((char*)inBuffer, inBufferSize);
            Json::Value setting;//
            Json::Reader reader;//解析
            if(!reader.parse(aoStr, setting)) {
              return -1;
            }            
            cmd_result_ = 0;   
            if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
                if(setting["signal_id"].asString() == "123456") {
                    //开门
                    boost::mutex::scoped_lock spLock(socket_mutex_);
                    uint8_t cmd[3] = { 1, 2, 4};
                    if(0 == Send(cmd, sizeof(cmd)))
                    {
                        if(0 == Receive())
                        {
                            //msg_buf和msg_index配合
                            cmd_result_ = 1;
                            return 0;
                        }
                    }
                
                }
            }
            break;
       }
    default:
        if((uint32_t)outBufferSize >= sizeof(int)) {
            *((int*)outBuffer) = cmd_result_;//无效命令
            bytesReturned = sizeof(int);
        } else {
            bytesReturned = 0;
        }
        break;
    }
    return 0;
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back ( std::make_shared<UdpExampleProvider>() );
    return std::move ( providerVec );
}

#endif

