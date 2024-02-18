#include "common_define.h"
#include "AGC150.h"
#include "UniDataDevice.cpp"


AGC150::AGC150()
{
    device_type_ = "agc150";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

AGC150::~AGC150()
{
}


bool AGC150::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    return UniDataDevice<AGC150_Data_t, SPModbus, RT_AGC150>::InitSetting(settingRoot);
}

void AGC150::RunCheckThreshold()
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

bool AGC150::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = AGC150_R4_501;
    modbus_read_input_registers(501, 27);
    return true;
}

bool AGC150::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case AGC150_R4_501:{
            memcpy(cData.r4_501, tab_reg, sizeof(uint16_t)*27);
            state = AGC150_R4_538;
            modbus_read_input_registers(538, 13);
	    break;
      }
      case AGC150_R4_538:{
            memcpy(cData.r4_538, tab_reg, sizeof(uint16_t)*13);
            state = AGC150_R4_1018;
            modbus_read_input_registers(1018, 3);
	    break;
      }
      case AGC150_R4_1018:{
            memcpy(cData.r4_1018, tab_reg, sizeof(uint16_t)*3);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float AGC150::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int AGC150::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<AGC150Provider>());
    return std::move(providerVec);
}

#endif
