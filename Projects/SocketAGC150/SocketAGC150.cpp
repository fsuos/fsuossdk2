#include "SocketAGC150.h"
#include "UniDataDevice.cpp"
#include <future>

SocketAGC150::SocketAGC150()
{
    device_type_ = "socketagc150";
    addr_ = 1;
}

SocketAGC150::~SocketAGC150()
{
    if(nullptr != pCtx){
        modbus_close(pCtx);
        modbus_free(pCtx);
    }
}

//static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
//{
//    memcpy(pFloat, buf+1, 2);
//    memcpy(pFloat+2, buf, 2);
//}


bool SocketAGC150::RefreshStatus()
{
    if(!ip_.empty())
    {
        std::cout<<"unicom ll11 state:"<<state<<std::endl;
        if(state != IDLE)
        {
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            boost::posix_time::time_duration  diff = now - lastTime;
            if( diff.total_seconds() > 60) {
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
            }else{
                //std::cout<<"unicom ll11 state:"<<state<<" no fresh "<<std::endl;
                return false;
            }
        }
        state = 1;
        if(nullptr == pCtx){
            pCtx = modbus_new_tcp(ip_.c_str(), port_);
            std::async( std::launch::async,
                        [&]() {
                            //std::cout<<"unicom ll11 refresh"<<std::endl;
                int nRet = modbus_connect(pCtx);
                if(-1 == nRet)
                {
                    //printf("connect failed\n");
                    modbus_close(pCtx);
                    modbus_free(pCtx);
                    pCtx = nullptr;
                    return;
                }
                uint16_t regs[43];
                if(-1 == modbus_read_input_registers(pCtx, 501, 27, regs))
                {
                    //std::cout<<"read error"<<std::endl;
                    modbus_close(pCtx);
                    modbus_free(pCtx);
                    pCtx = nullptr;
                    return;
                }
                memcpy(cData.r4_501, regs, sizeof(uint16_t)*27);
                    if(-1 == modbus_read_input_registers(pCtx, 538, 13, regs))
                    {
                        std::cout<<"read error"<<std::endl;
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
					memcpy(cData.r4_538, regs, sizeof(uint16_t)*13);
                    if(-1 == modbus_read_input_registers(pCtx, 1018, 3, regs))
                    {
                        std::cout<<"read error"<<std::endl;
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
					memcpy(cData.r4_1018, regs, sizeof(uint16_t)*3);                
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
                RoundDone();
                return;
            });
        }        
    }    
    return false;
}

void SocketAGC150::RunCheckThreshold()
{
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_1_3", "05023", SIGNAME_05023, "油机不在自动模式告警", ((cData.r4_1018[1] >> 3) & 0x1) == 0, 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_0_6", "05009", SIGNAME_05009, "油机启动", ((cData.r4_1018[0] >> 6) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_0", "05024", SIGNAME_05024, "通信错误", ((cData.r4_1018[2]) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_1", "05024", SIGNAME_05024, "警告/黄色报警", ((cData.r4_1018[2] >> 1) & 0x1), 2);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_2", "05024", SIGNAME_05024, "警告/红色报警", ((cData.r4_1018[2] >> 2) & 0x1), 3);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_3", "05001", SIGNAME_05001, "超速", ((cData.r4_1018[2] >> 3) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_4", "05013", SIGNAME_05004, "冷却水温度1", ((cData.r4_1018[2] >> 4) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05004, "冷却水温度2", ((cData.r4_1018[2] >> 5) & 0x1), 2);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05010, "油压1", ((cData.r4_1018[2] >> 6) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05010, "油压2", ((cData.r4_1018[2] >> 7) & 0x1), 2);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05013, "油温1", ((cData.r4_1018[2] >> 8) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05013, "油温2", ((cData.r4_1018[2] >> 9) & 0x1), 2);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05015, "冷却水液位1", ((cData.r4_1018[2] >> 10) & 0x1), 1);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05013", SIGNAME_05015, "冷却水液位2", ((cData.r4_1018[2] >> 11) & 0x1), 2);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05024", SIGNAME_05024, "排气缸差温1", ((cData.r4_1018[2] >> 12) & 0x1), 4);
	CheckThresholdBool(SMDController::LEVEL1, "4_1018_2_5", "05024", SIGNAME_05024, "排气缸差温2", ((cData.r4_1018[2] >> 13) & 0x1), 5);

}



bool SocketAGC150::Init ( std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot )
{
    SMDSocketDevice::Init ( controller, data_id, settingRoot );
    return true;
}

bool SocketAGC150::process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size)
{
        return true;
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back ( std::make_shared<SocketAGC150Provider>() );
    return std::move ( providerVec );
}

#endif
