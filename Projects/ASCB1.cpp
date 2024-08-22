#include "ASCB1.h"
#include "UniDataDevice.cpp"
#include <iostream>

ASCB1::ASCB1()
{
    device_type_ = "ascb1";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}
 
ASCB1::~ASCB1()
{
}


void ASCB1::RunCheckThreshold()
{
	//alarm1
    CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压量程", cData.alarm1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流量程", cData.alarm1>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","接线方式", cData.alarm1>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001","状态告警", "电压变比", cData.alarm1>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","漏电流变比", cData.alarm1>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","漏电报警类型", cData.alarm1>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","漏电预警值", cData.alarm1>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","漏电报警值", cData.alarm1>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","漏电报警时间", cData.alarm1>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度1报警类型", cData.alarm1>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度1报警类型", cData.alarm1>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度1预警值", cData.alarm1>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度1预警值", cData.alarm1>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001","状态告警", "温度1警时间", cData.alarm1>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度2报警类型", cData.alarm1>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度2预警值", cData.alarm1>>15&0x1, 1);
//alarm2
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度2报警值", cData.alarm2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度2警时间", cData.alarm2>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度3报警类型", cData.alarm2>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度3预警值", cData.alarm2>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度3报警值", cData.alarm2>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度3警时间", cData.alarm2>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度4报警类型", cData.alarm2>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001","状态告警","温度4预警值", cData.alarm2>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度4报警值", cData.alarm2>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","温度4警时间", cData.alarm2>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过压预警值", cData.alarm2>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过压报警值", cData.alarm2>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过压报警时间", cData.alarm2>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","欠压报警类型", cData.alarm2>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","欠压预警值", cData.alarm2>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","欠压报警值", cData.alarm2>>15&0x1, 1);
//alarm3
CheckThresholdBool(1, "0777001", "0777001", "状态告警","欠压报警类型", cData.alarm3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过流预警值", cData.alarm3>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过流报警值", cData.alarm3>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过流报警时间", cData.alarm3>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过功率报警类型", cData.alarm3>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过功率预警值", cData.alarm3>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过功率报警值", cData.alarm3>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","过功率报警时间", cData.alarm3>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001","状态告警", "短路报警类型", cData.alarm3>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","短路预警值", cData.alarm3>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","短路报警值", cData.alarm3>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","短路报警时间", cData.alarm3>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压不平衡报警类型", cData.alarm3>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压不平衡预警值", cData.alarm3>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压不平衡报警值", cData.alarm3>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压不平衡报警时间", cData.alarm3>>15&0x1, 1);
//alarm4
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流不平衡报警类型", cData.alarm4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流不平衡预警值", cData.alarm4>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流不平衡报警值", cData.alarm4>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流不平衡报警时间", cData.alarm4>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压逆相报警类型", cData.alarm4>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压逆相预警值", cData.alarm4>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压逆相报警值", cData.alarm4>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电压逆相报警时间", cData.alarm4>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流逆相报警类型", cData.alarm4>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流逆相预警值", cData.alarm4>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流逆相报警值", cData.alarm4>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","电流逆相报警时间", cData.alarm4>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","功率因数低报警类型", cData.alarm4>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","功率因数低预警值", cData.alarm4>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","功率因数低报警值", cData.alarm4>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警","功率因数低报警时间", cData.alarm4>>15&0x1, 1);

}

bool ASCB1::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = ASCB1_R3_0x000A;
    modbus_read_registers(0x000A, 16);
    return true;
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}


bool ASCB1::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case ASCB1_R3_0x000A:{
            memcpy(cData.r3_0x000A, tab_reg, sizeof(uint16_t)*16);
            state = ASCB1_R3_0x001A;
            modbus_read_registers(0x001A, 114);
	    break;
      }
      case ASCB1_R3_0x001A:{
			  for(int i=0; i<57; i++)  {
				   //memcpy(cData.r3_0x001A,tab_reg,sizeof(float)*57);
			copy_to_float(tab_reg + 2*i, (uint8_t*)&cData.r3_0x001A[i]);
		}
            RoundDone();
	    return false;
      }
   
   return true;
}
}

int ASCB1:: DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) 
{
	 switch(ioControlCode) {
		case 320:{
			//set DO
//            cmd_result_ = -1;
			std::string doId((char*)inBuffer, inBufferSize);
			
			OpenPort();
                    if(doId == "321500100001") {
						//开机
						uint16_t tArg = 1;
						modbus_write_registers(0x19,1,&tArg);
					}else if(doId == "321300101001"){
						//关机
						OpenPort();
						uint16_t tArg = 0;
						modbus_write_registers(0x19,1,&tArg);
					}
						 break;
                    }
//					default: {
//        if((uint32_t)outBufferSize >= sizeof(int)) {
//            *((int*)outBuffer) = cmd_result_;//无效命令
//            bytesReturned = sizeof(int);
//        } else {
//            bytesReturned = 0;
//					}
//        break;
//		}
}
 return 0;
}
float ASCB1::Get_Value(uint32_t data_id, const std::string& var_name) const
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





#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<ASCB1Provider>());
    return std::move(providerVec);
}

#endif
