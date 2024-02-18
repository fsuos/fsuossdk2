#include "common_define.h"
#include "EvoCyberMateDXModbus.h"
#include "UniDataDevice.cpp"


EvoCyberMateDXModbus::EvoCyberMateDXModbus()
{
    device_type_ = "evocybermatedxmodbus";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

EvoCyberMateDXModbus::~EvoCyberMateDXModbus()
{
}


bool EvoCyberMateDXModbus::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    return UniDataDevice<EvoCyberMateDXModbus_Data_t, SPModbus, RT_EvoCyberMateDXModbus>::InitSetting(settingRoot);
}

void EvoCyberMateDXModbus::RunCheckThreshold()
{
}

bool EvoCyberMateDXModbus::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = EvoCyberMateDXModbus_R3_256;
    modbus_read_registers(256, 40);
    return true;
}

bool EvoCyberMateDXModbus::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case EvoCyberMateDXModbus_R3_256:{
            memcpy(cData.r3_256, tab_reg, sizeof(uint16_t)*40);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float EvoCyberMateDXModbus::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int EvoCyberMateDXModbus::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
          for(auto it = setting.begin(); it != setting.end(); it++) {
            if((*it)["signal_id"] != Json::nullValue && (*it)["signal_id"].type()  != Json::nullValue) {
                if((*it)["signal_id"].asString() != "") {
                    SetAOAlarmRule((*it)["signal_id"].asString(), (*it));
                }
            }
        }
       
    }
    break;
    case 320: {
        state = 320;
        cmd_result_ = -1;
        OpenPort();
        //320是SET_DO
        std::string doId((char*)inBuffer, inBufferSize);
        break;
    }

    default:
        if((uint32_t)outBufferSize >= sizeof(int)) {
            *((int*)outBuffer) = 2;//无效命令
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
    providerVec.push_back(std::make_shared<EvoCyberMateDXModbusProvider>());
    return std::move(providerVec);
}

#endif
