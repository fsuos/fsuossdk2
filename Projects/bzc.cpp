#include "bzc.h"
#include "UniDataDevice.cpp"
#include <arpa/inet.h>


bzc::bzc()
{
    device_type_ = "bzc";
    baud_rate_ = 115200;
    addr_ = 1;
    //save_interval_ = 600;
}

bzc::~bzc()
{
}


bool bzc::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
	
    return UniDataDevice<bzc_Data_t, SPModbus, RT_bzc>::InitSetting(settingRoot);
}


bool bzc::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
	CallVoltage();
//    state = bzc_R3_7;
//    modbus_read_registers(7, 4);
	return true;
}
	 

bool bzc::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case bzc_VOLTAGE:{
//            memcpy(cData.r3_7, tab_reg, sizeof(uint16_t)*4);
				 CallGroupVoltage();
            state = bzc1_VOLTAGE;
//            modbus_read_registers(366, 44);
	    break;
      }
      case bzc1_VOLTAGE:{
            //memcpy(cData.r3_366, tab_reg, sizeof(uint16_t)*44);
			 state = bzc_IDLE;
            RoundDone();
	    return false;
      }
   }
   return true;
}


float bzc::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int bzc::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
	}
    
	else if(doId == "123456"){
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

void bzc::CallVoltage()
{
    msg_index = 0;
    unsigned char cmd[] = {0xa5, 01, 03, 01, addr_,0x00};
	cmd[5] = GetMMCheckSum(cmd , 5);
    SendData(cmd, 6);
	index = 4;
	cmd[index] += 1;
	state = bzc_VOLTAGE;
    lastTime = boost::posix_time::second_clock::local_time();
}

void bzc::CallGroupVoltage()
{
    //msg_index = 0;
    unsigned char cmd[] = {0xa5, 01,10 , 03, addr_,'1','2','3','4','5','6',0x00};
	cmd[11] = GetMMCheckSum(cmd , 11);
    SendData(cmd, 12);
	index = 4;
	cmd[index] += 1;
    state = bzc1_VOLTAGE;
    lastTime = boost::posix_time::second_clock::local_time();
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<bzcProvider>());
    return std::move(providerVec);
}

#endif
