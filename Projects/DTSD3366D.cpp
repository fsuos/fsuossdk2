#include "DTSD3366D.h"
#include "UniDataDevice.cpp"


DTSD3366D::DTSD3366D()
{
    device_type_ = "dtsd3366d";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

DTSD3366D::~DTSD3366D()
{
}


bool DTSD3366D::InitSetting(const Json::Value &settingRoot)
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
	return UniDataDevice<DTSD3366D_Data_t, SPModbus, RT_DTSD3366D>::InitSetting(settingRoot);
}

void DTSD3366D::RunCheckThreshold()
{
 
   CheckThresholdBool(1, "0777001", "0777001", "停单告警", "停单告警", (cData.r3_7[0]>>2)&0x1, 1);
   CheckThreshold("voltage","电压", ((float)cData.r3_7[0])/100, signal_index_++);
}

bool DTSD3366D::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = DTSD3366D_R3_7;
    modbus_read_registers(7, 4);
    return true;
}

bool DTSD3366D::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case DTSD3366D_R3_7:{
            memcpy(cData.r3_7, tab_reg, sizeof(uint16_t)*4);
            state = DTSD3366D_R3_366;
            modbus_read_registers(366, 44);
	    break;
      }
      case DTSD3366D_R3_366:{
            memcpy(cData.r3_366, tab_reg, sizeof(uint16_t)*44);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float DTSD3366D::Get_Value(uint32_t data_id, const std::string& var_name) const
{
    if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }

   if(var_name == "Ua"){
     return cData.r3_7[0];
   }
   else if(var_name == "Ub"){
     return cData.r3_7[1];
   }
    throw std::out_of_range("不支持变量");
}



int DTSD3366D::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    
		    if((*it)["signal_id"].asString() == "1234567"){
                      //测试1
                      SetAOReportSetting("1234567", setting);
                    }
    
		    else if((*it)["signal_id"].asString() == "1234568"){
                      //测试2
                      SetAOReportSetting("1234568", setting);
                    }
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
    
	if(doId == "1234567"){
	//测试3
	}
    
	else if(doId == "1234568"){
	//测试4
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
    return 0;
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<DTSD3366DProvider>());
    return std::move(providerVec);
}

#endif
