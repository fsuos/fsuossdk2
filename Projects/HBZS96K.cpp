#include "common_define.h"
#include "HBZS96K.h"
#include "UniDataDevice.cpp"


HBZS96K::HBZS96K()
{
    device_type_ = "hbzs96k";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

HBZS96K::~HBZS96K()
{
}


bool HBZS96K::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
        if(settingRoot["appSetting"]["ct"] != Json::nullValue) {
            ct_ = atoi(settingRoot["appSetting"]["ct"].asString().c_str());
        }
        if(settingRoot["appSetting"]["has_a"] != Json::nullValue) {
            has_a_ = atoi(settingRoot["appSetting"]["has_a"].asString().c_str());
        }
    }
    return UniDataDevice<HBZS96K_Data_t, SPModbus, RT_HBZS96K>::InitSetting(settingRoot);
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

void HBZS96K::RunCheckThreshold()
{
}

bool HBZS96K::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = HBZS96K_R3_10;
    modbus_read_registers(10, 14);
    return true;
}

bool HBZS96K::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case HBZS96K_R3_10:{
            memcpy(cData.r3_10, tab_reg, sizeof(uint16_t)*14);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float HBZS96K::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int HBZS96K::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<HBZS96KProvider>());
    return std::move(providerVec);
}

#endif
