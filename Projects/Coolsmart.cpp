#include "Coolsmart.h"
#include "UniDataDevice.cpp"


Coolsmart::Coolsmart()
{
    device_type_ = "coolsmart";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

Coolsmart::~Coolsmart()
{
}


bool Coolsmart::InitSetting(const Json::Value &settingRoot)
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
    return UniDataDevice<Coolsmart_Data_t, SPModbus, RT_Coolsmart>::InitSetting(settingRoot);
}

void Coolsmart::RunCheckThreshold()
{
 //alarm1
    CheckThresholdBool(1, "0777001", "0777001", "状态告警", "高压告警", (cData.alarm1&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "高压锁定", ((cData.alarm1>>1)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "低压告警", ((cData.alarm1>>2)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "低压锁定", ((cData.alarm1>>3)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "排气高温", ((cData.alarm1>>4)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "排气高温锁定", ((cData.alarm1>>5)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "排气低过热度", ((cData.alarm1>>6)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "排气低过热度锁定", ((cData.alarm1>>7)&0x1) == 1, signal_index_++);
	
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回风高温", ((cData.alarm1>>8)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回风高湿", ((cData.alarm1>>9)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回风地湿", ((cData.alarm1>>10)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回风低温", ((cData.alarm1>>11)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "送风高温", ((cData.alarm1>>12)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "送风低温", ((cData.alarm1>>13)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回温故障", ((cData.alarm1>>14)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回湿故障", ((cData.alarm1>>15)&0x1) == 1, signal_index_++);
	//alarm2
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "送温故障", (cData.alarm2&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "排温故障", ((cData.alarm2>>1)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "风机故障", ((cData.alarm2>>2)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "地板溢水", ((cData.alarm2>>3)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "远程关机", ((cData.alarm2>>4)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电源丢失", ((cData.alarm2>>5)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "过滤网堵", ((cData.alarm2>>6)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "烟感告警", ((cData.alarm2>>7)&0x1) == 1, signal_index_++);
	
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电源故障", ((cData.alarm2>>8)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "盘管冻结", ((cData.alarm2>>9)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "气流丢失", ((cData.alarm2>>10)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "加湿高水位", ((cData.alarm2>>11)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "加湿器故障", ((cData.alarm2>>12)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "制冷剂不足", ((cData.alarm2>>13)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "冷凝器需维护", ((cData.alarm2>>14)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵不可用", ((cData.alarm2>>15)&0x1) == 1, signal_index_++);
	//alarm3
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵入口压力故障", (cData.alarm3&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵出口压力故障", ((cData.alarm3>>1)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵环温故障", ((cData.alarm3>>2)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵过温故障", ((cData.alarm3>>3)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵过温锁定", ((cData.alarm3>>4)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵高扬程", ((cData.alarm3>>5&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵低扬程", ((cData.alarm3>>6)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵高扬程锁定", ((cData.alarm3>>7)&0x1) == 1, signal_index_++);
	
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵低扬程锁定", ((cData.alarm3>>8)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵驱动故障", ((cData.alarm3>>9)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "泵通信故障", ((cData.alarm3>>10)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "冷凝压力故障", ((cData.alarm3>>11)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "冷凝风机过温",((cData.alarm3>>12)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "冷凝风机过温锁定", ((cData.alarm3>>13)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "冷凝驱动故障", ((cData.alarm3>>14)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "冷凝通信故障", ((cData.alarm3>>15)&0x1) == 1, signal_index_++);
	//alarm4
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "吸气温度故障", (cData.alarm4&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "低压传感器故障", ((cData.alarm4>>1)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "机组地址重复", ((cData.alarm4>>2)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "从机丢失", ((cData.alarm4>>3)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "群控本机离线", ((cData.alarm4>>4)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "压机通信故障", ((cData.alarm4>>5)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "压机驱动故障", ((cData.alarm4>>6)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "压机驱动故障锁定", ((cData.alarm4>>7)&0x1) == 1, signal_index_++);
	
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "压机驱动过欠压", ((cData.alarm4>>8)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "高压传感器故障", ((cData.alarm4>>9)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "高压异常", ((cData.alarm4>>10)&0x1) == 1, signal_index_++);
	CheckThresholdBool(1, "0777001", "0777001", "状态告警", "节能卡故障", ((cData.alarm4>>11)&0x1) == 1, signal_index_++);
	
}

bool Coolsmart::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = Coolsmart_R3_7;
    modbus_read_registers(7, 4);
    return true;
}

bool Coolsmart::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case Coolsmart_R3_7:{
            memcpy(cData.r3_7, tab_reg, sizeof(uint16_t)*4);
            state = Coolsmart_R3_366;
            modbus_read_registers(366, 44);
	    break;
      }
      case Coolsmart_R3_366:{
            memcpy(cData.r3_366, tab_reg, sizeof(uint16_t)*44);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float Coolsmart::Get_Value(uint32_t data_id, const std::string& var_name) const
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



int Coolsmart::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<CoolsmartProvider>());
    return std::move(providerVec);
}

#endif
