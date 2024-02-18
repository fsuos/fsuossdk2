#include "common_define.h"
#include "ETPower.h"
#include "UniDataDevice.cpp"


ETPower::ETPower()
{
    device_type_ = "etpower";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

ETPower::~ETPower()
{
}


bool ETPower::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
        if(settingRoot["appSetting"]["has_a"] != Json::nullValue) {
            has_a_ = atoi(settingRoot["appSetting"]["has_a"].asString().c_str());
        }
        if(settingRoot["appSetting"]["has_b"] != Json::nullValue) {
            has_b_ = atoi(settingRoot["appSetting"]["has_b"].asString().c_str());
        }
        if(settingRoot["appSetting"]["has_c"] != Json::nullValue) {
            has_c_ = atoi(settingRoot["appSetting"]["has_c"].asString().c_str());
        }
    }
    return UniDataDevice<ETPower_Data_t, SPModbus, RT_ETPower>::InitSetting(settingRoot);
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

void ETPower::RunCheckThreshold()
{
    float fA = 0.0,fB = 0.0,fC = 0.0;
copy_to_float(cData.r3_1280, (uint8_t*)&fA);
copy_to_float(cData.r3_1280+2, (uint8_t*)&fA);
copy_to_float(cData.r3_1280+4, (uint8_t*)&fA);
CheckThresholdBool(1, "p41_44_0","321415001", "输入电源中断", "输入电源中断 UA:" + boost::lexical_cast<std::string>(fA) + " UB:" + boost::lexical_cast<std::string>(fB)
+ " UC:" + boost::lexical_cast<std::string>(fC), (has_a_ && (fA < 120) ) || 
                ( has_b_ && (fB < 120 )) || ( has_c_ && (fC < 120 )), 1);
}

bool ETPower::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = ETPower_R3_1280;
    modbus_read_registers(1280, 24);
    return true;
}

bool ETPower::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case ETPower_R3_1280:{
            memcpy(cData.r3_1280, tab_reg, sizeof(uint16_t)*24);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float ETPower::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int ETPower::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<ETPowerProvider>());
    return std::move(providerVec);
}

#endif
