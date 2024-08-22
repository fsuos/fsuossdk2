#include "common_define.h"
#include "DPJG1K.h"
#include "UniDataDevice.cpp"


DPJG1K::DPJG1K()
{
    device_type_ = "dpjg1k";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

DPJG1K::~DPJG1K()
{
}


bool DPJG1K::InitSetting(const Json::Value &settingRoot)
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
    return UniDataDevice<DPJG1K_Data_t, SPModbus, RT_DPJG1K>::InitSetting(settingRoot);
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

void DPJG1K::RunCheckThreshold()
{    
    switch(b_mode_)
    {
        case 1://联通
        {
            break;
        }
        case 2://电信
        {
            break;
        }
    }
}

bool DPJG1K::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
        state = DPJG1K_R3_10;
        modbus_read_registers(10, 14);
    return true;
}

bool DPJG1K::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case DPJG1K_R3_10:{
            memcpy(cData.r3_10, tab_reg, sizeof(uint16_t)*14);
            RoundDone();
            return false;
      }
   }
   return true;
}


float DPJG1K::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int DPJG1K::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
            if(setting["signal_id"].asString() != "") {
            }
        }
    }
    break;
    case 320: {
        state = 320;
        cmd_result_ = -1;
        OpenPort();
        //320是SET_DO
        std::string aoStr((char*)inBuffer, inBufferSize);
        Json::Value setting;//
        Json::Reader reader;//解析
        if(!reader.parse(aoStr, setting)) {
          return -1;
        }
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
            if(setting["signal_id"].asString() != "") {
            }
        }
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
    return UniDataDevice<DPJG1K_Data_t, SPModbus, RT_DPJG1K>::DeviceIoControl(ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned);
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<DPJG1KProvider>());
    return std::move(providerVec);
}

#endif
