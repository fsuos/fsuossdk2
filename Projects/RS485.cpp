#include "RS485.h"
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "UniDataDevice.cpp"

RS485::RS485()
{
    baud_rate_ = 115200;
}

bool RS485::RefreshStatus()
{
    if(state != IDLE) {
        //支持重发上一条命令
        if(iRetryCount++ < 3) {
            switch(state) {
            case RS485_VOLTAGE: {
                CallVoltage();
                break;
            }
            case GROUP_VOLTAGE: {
                CallGroupVoltage();
                break;
            }
           
            }
            return true;
        }
    }
    iRetryCount = 0;
    SMDSPDevice::RefreshStatus();
    CallVoltage();
    return true;
}
void RS485::CallVoltage()
{
    msg_index = 0;
    unsigned char cmd[] = {0xa5, 0, 3, 0x01, addr_,0x00};
	cmd[5] = sizeof(cmd);
    SendData(cmd, 6);
	index = 4;
	cmd[index] += 1;
    state = RS485_VOLTAGE;
    lastTime = boost::posix_time::second_clock::local_time();
}

void RS485::CallGroupVoltage()
{
    msg_index = 0;
    unsigned char cmd[] = {0xa5, 0,10 , 0x03, addr_,'1','2','3','4','5','6',0x00};
    cmd[11] = sizeof(cmd);
    SendData(cmd, 12);
	index = 4;
	cmd[index] += 1;
    state = GROUP_VOLTAGE;
    lastTime = boost::posix_time::second_clock::local_time();
}



float RS485::Get_Value(uint32_t data_id, const std::string &var_name) const
{
   if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }

    
    throw std::out_of_range("不支持变量");
}

#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<RS485Provider>());
    return std::move(providerVec);
}

#endif