#include "common_define.h"
#include "MediaMAV.h"
#include "UniDataDevice.cpp"


MediaMAV::MediaMAV()
{
    device_type_ = "mediamav";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

MediaMAV::~MediaMAV()
{
}


bool MediaMAV::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    return UniDataDevice<MediaMAV_Data_t, SPModbus, RT_MediaMAV>::InitSetting(settingRoot);
}

void MediaMAV::RunCheckThreshold()
{
}

bool MediaMAV::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = MediaMAV_R1_0;
    modbus_read_bits(0, 114);
    return true;
}

bool MediaMAV::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case MediaMAV_R1_0:{
	    memcpy(cData.b1_0, tab_reg, sizeof(uint8_t)*114);
            state = MediaMAV_R3_0;
            modbus_read_registers(0, 61);
	    break;
      }
      case MediaMAV_R3_0:{
            memcpy(cData.r3_0, tab_reg, sizeof(uint16_t)*61);
            state = MediaMAV_R3_62;
            modbus_read_registers(62, 6);
	    break;
      }
      case MediaMAV_R3_62:{
            memcpy(cData.r3_62, tab_reg, sizeof(uint16_t)*6);
            state = MediaMAV_R3_168;
            modbus_read_registers(168, 1);
	    break;
      }
      case MediaMAV_R3_168:{
            memcpy(cData.r3_168, tab_reg, sizeof(uint16_t)*1);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float MediaMAV::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int MediaMAV::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<MediaMAVProvider>());
    return std::move(providerVec);
}

#endif
