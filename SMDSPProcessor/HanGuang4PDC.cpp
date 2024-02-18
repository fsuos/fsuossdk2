#include "common_define.h"
#include "HanGuang4PDC.h"
#include "UniDataDevice.cpp"


HanGuang4PDC::HanGuang4PDC()
{
    device_type_ = "hanguang4pdc";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

HanGuang4PDC::~HanGuang4PDC()
{
}


bool HanGuang4PDC::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    return UniDataDevice<HanGuang4PDC_Data_t, SPModbus, RT_HanGuang4PDC>::InitSetting(settingRoot);
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

void HanGuang4PDC::RunCheckThreshold()
{
}

bool HanGuang4PDC::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = HanGuang4PDC_R3_0;
    modbus_read_registers(0, 12);
    return true;
}

bool HanGuang4PDC::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case HanGuang4PDC_R3_0:{
            memcpy(cData.r3_0, tab_reg, sizeof(uint16_t)*12);
            state = HanGuang4PDC_R3_12;
            modbus_read_registers(12, 10);
	    break;
      }
      case HanGuang4PDC_R3_12:{
            memcpy(cData.r3_12, tab_reg, sizeof(uint16_t)*10);
            state = HanGuang4PDC_R3_22;
            modbus_read_registers(22, 10);
	    break;
      }
      case HanGuang4PDC_R3_22:{
            memcpy(cData.r3_22, tab_reg, sizeof(uint16_t)*10);
            state = HanGuang4PDC_R3_32;
            modbus_read_registers(32, 10);
	    break;
      }
      case HanGuang4PDC_R3_32:{
            memcpy(cData.r3_32, tab_reg, sizeof(uint16_t)*10);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float HanGuang4PDC::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int HanGuang4PDC::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<HanGuang4PDCProvider>());
    return std::move(providerVec);
}

#endif
