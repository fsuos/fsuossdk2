#include "HBZS96K.h"
#include "common_define.h"
#include "UniDataDevice.cpp"

HBZS96K::HBZS96K()
{
    device_type_ = "hbzs96k";
    baud_rate_ = 19200;
    version_ = 0x21;
    addr_ = 1;
}


bool HBZS96K::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = STATE_2c_41;
    write_pmbus_cmd ( 0x2c, 0x41 );
    return true;
}

bool HBZS96K::process_payload()
{
    if(state == AC_Control_Code::AC_START) {
        if(pPMBus->cid2 == 0) {
            cmd_result_ = 1;
        } else {
            cmd_result_ = 3;
        }
        Reset();
        return false;
    } else if(state == AC_Control_Code::AC_STOP) {
        if(pPMBus->cid2 == 0) {
            cmd_result_ = 1;
        } else {
            cmd_result_ = 3;
        }
        Reset();
        return false;
    } else if(state == AC_Control_Code::AC_SET_TEMPERATURE) {
        if(pPMBus->cid2 == 0) {
            cmd_result_ = 1;
        } else {
            cmd_result_ = 3;
        }
        Reset();
        return false;
    }
    switch ( state ) {
    case STATE_2c_41: {
        if(pPMBus->len/2 >= 52) {
            //extract data
            //unsigned short * di = ( unsigned short* ) pPMBus->data;
            uint16_t tDi;
            memcpy(&tDi, pPMBus->data, 2);
            cData.DC VoltageA = ( float ( ntohs ( tDi ) ) ) / 10;
            memcpy(&tDi, pPMBus->data + 2, 2);
            cData.DC currentA = ( float ( ntohs ( tDi ) ) ) / 10;
            memcpy(&tDi, pPMBus->data + 4, 2);
            cData.DC powerA = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 6, 2);
			cData.DC energyA = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 8, 2);
			cData.DC current demandA = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 10, 2);
			cData.DC power demandA = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 12, 2);
			cData.DC VoltageB = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 14, 2);
			cData.DC currentB = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 16, 2);
			cData.DC powerB = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 18, 2);
			cData.DC energyB = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 20, 2);
			cData.DC current demandB = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 22, 2);
			cData.DC power demandB = ( float ( ntohs ( tDi ) ) ) / 10;
			memcpy(&tDi, pPMBus->data + 24, 2);
            state = STATE_2c_43;
            write_pmbus_cmd ( 0x2c, 0x43 );
            break;
        } else {
            return false;
        }
    }
	}

//    case STATE_60_43: {
//        if(pPMBus->len/2 >= 2) {
//            unsigned char * p = pPMBus->data;
//            cData.air_state = *p;
//            state = STATE_60_47;
//            write_pmbus_cmd ( 0x60, 0x47 );
//            break;
//        } else {
//            return false;
//        }
//    }
//    case STATE_60_47: {
//        if(pPMBus->len/2 >= 6) {
//            //unsigned short * di = ( unsigned short* ) pPMBus->data;
//            uint16_t tDi;
//            memcpy(&tDi, pPMBus->data, 2);
//            cData.temperature =  ( float ( ntohs ( tDi ) ) ) / 10; //开机温度
//            memcpy(&tDi, pPMBus->data+2, 2);
//            cData.humidity =  ( float ( ntohs ( tDi ) ) ) / 10; //关机湿度
//            memcpy(&tDi, pPMBus->data+4, 2);
//            cData.set_temp = ( float ( ntohs ( tDi ) ) ) / 10; //温度设定点
//            memcpy(&tDi, pPMBus->data+6, 2);
//            cData.temp_pric =  ( float ( ntohs ( tDi ) ) ) / 10; //温度偏差
//            memcpy(&tDi, pPMBus->data+8, 2);
//            cData.set_humid =  ( float ( ntohs ( tDi ) ) ) / 10; //湿度设定点
//            memcpy(&tDi, pPMBus->data+10, 2);
//            cData.humid_pric = ( float ( ntohs ( tDi ) ) ) / 10; //湿度偏差
//            memcpy(&tDi, pPMBus->data+12, 2);
//            state = STATE_60_82;
//            write_pmbus_cmd ( 0x60, 0x82 );
//            break;
//        } else {
//            return false;
//        }
//    }
    case STATE_2c_43: {
        if(pPMBus->len/2 >= 145) {
            unsigned char * p = pPMBus->data;
            //jump p
            p++;
            cData.statusA =  *p & 0x01;//A开关状态
            cData.statusB =  ( *p >> 1 ) & 0x01; //B开关状态
			cData.number_of_circuits =  *p >> 2 ) & 0x01;//回路数
			state = STATE_2c_44;
			write_pmbus_cmd ( 0x2c, 0x44 );
		}
//            cData.cool_status =  ( *p >> 2 ) & 0x01; //制冷状态
//            cData.heat_status =  ( *p >> 3 ) & 0x01; //加热状态
//            cData.humid_status = ( *p >> 4 ) & 0x01; //加湿状态
//            cData.dehumid_status =  ( *p >> 5 ) & 0x01; //除湿状态
            
//            p++;
//
//            cData.custom_alarm1 =  *p & 0x01;                   //用户自定义告警1
//            cData.custom_alarm2 = ( *p >> 1 ) & 0x01;           //用户自定义告警2
//            cData.main_fan_mainten_alarm = ( *p >> 2 ) & 0x01;  //主风机维护告警
//            cData.humid_mainten_alarm = ( *p >> 3 ) & 0x01;     //加湿器维护报警
//            cData.filter_mainten_alarm =  ( *p >> 4 ) & 0x01;   //过滤网维护报警
//            cData.com_failer_alarm = ( *p >> 5 ) & 0x01;        //通讯故障报警
//            cData.coil_freeze_alarm =  ( *p >> 6 ) & 0x01;      //盘管冻结告警
//            cData.humid_fault_alarm =  ( *p >> 7 ) & 0x01;      //加湿器故障报警

//            p++;
//            cData.sensor_miss_alarm = *p & 0x01;                //传感器板丢失告警
//            cData.gas_temp_fault_alarm = ( *p >> 1 ) & 0x01;    //排气温度故障告警
//            cData.power_miss_fault_alarm = ( *p >> 2 ) & 0x01;  //电源丢失故障告警
//            cData.power_undervol_alarm = ( *p >> 3 ) & 0x01;    //电源过欠压报警
//            cData.power_phase_alarm  = ( *p >> 4 ) & 0x01;      //电源缺相报警
//            cData.power_freq_alarm = ( *p >> 5 ) & 0x01;        //电源频率偏移报警
//            cData.floor_overflow_alarm =  ( *p >> 6 ) & 0x01;   //地板溢水报警
//            cData.save_card_failure =  ( *p >> 7 ) & 0x01;      //节能卡故障
//            state = STATE_60_83;

            RoundDone();
            return false;
            //write_pmbus_cmd ( 0x60, 0x83 );
            break;
        } else {
            return false;
        }
    
    case STATE_2c_44: 
        if(pPMBus->len/2 >= 29) {
            unsigned char * p = pPMBus->data;
//            cData.engine_status = *p++;
//            cData.lock_status = *p;
p++;
            //cData.alert_status =  *p++; //告警状态
            cData.A_line_incoming_fuse =  *p & 0x01; //A路进线熔断器
            cData.A_circuit_lightning_arrester =  ( *p >> 1 ) & 0x01; //A路防雷器
            cData.B_circuit_lightning_arrester =  ( *p >> 2 ) & 0x01; //B路防雷器
            cData.A_line_incoming_voltage =  ( *p >> 3 ) & 0x01; //A路进线电压
            cData.A_line_incoming_current = ( *p >> 4 ) & 0x01; //A路进线电流
            cData.A_line_incoming_power =  ( *p >> 5 ) & 0x01; //A路进线功率
            cData.B_circuit_lightning_arrester =  ( *p >> 6 ) & 0x01; //B路进线熔断器
            cData.B_line_incoming_voltage = ( *p >> 7 ) & 0x01; //B路进线电压
			cData.B_line_incoming_current = ( *p >> 8 ) & 0x01;//B路进线电流
			cData.B_line_incoming_power = ( *p >> 9 ) & 0x01;//B路进线功率
			cData.Branch_circuit_fuse_alarm = ( *p >> 10 ) & 0x01;//支路熔断告警
            RoundDone();
            return false;
        }
    }
    
    return true;
}

int HBZS96K::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
{
//    switch(ioControlCode) {
//    case AC_Control_Code::AC_START: {
//        state = AC_Control_Code::AC_START;
//        cmd_result_ = -1;
//        OpenPort();
//        uint8_t cd = 0x10;
//        write_pmbus_cmd ( 0x60, 0x45, &cd, 1);
//        if((uint32_t)outBufferSize >= sizeof(int)) {
//            *((int*)outBuffer) = 1;//通知取状态
//            bytesReturned = sizeof(int);
//        }
//    }
//    break;
//    case AC_Control_Code::AC_STOP: {
//        state = AC_Control_Code::AC_STOP;
//        cmd_result_ = -1;
//        OpenPort();
//        uint8_t cd = 0x1f;
//        write_pmbus_cmd ( 0x60, 0x45, &cd, 1);
//        if((uint32_t)outBufferSize >= sizeof(int)) {
//            *((int*)outBuffer) = 1;//通知取状态
//            bytesReturned = sizeof(int);
//        }
//        break;
//    }
//    case AC_Control_Code::AC_SET_TEMPERATURE: {
//        if(inBufferSize == sizeof(uint16_t)) {
//            state = AC_Control_Code::AC_SET_TEMPERATURE;
//            cmd_result_ = -1;
//            OpenPort();
//            uint8_t cd[3];
//            cd[0] = 0x82;
//            last_data_ = htons(10* (*(uint16_t*)inBuffer));
//            memcpy(cd+1, &last_data_, 2);
//            write_pmbus_cmd ( 0x60, 0x49, cd, 3);
//            if((uint32_t)outBufferSize >= sizeof(int)) {
//                *((int*)outBuffer) = 1;//通知取状态
//                bytesReturned = sizeof(int);
//            }
//        }
//        break;
//    }
//    case AC_Control_Code::AC_START_RESULT:
//    case AC_Control_Code::AC_STOP_RESULT:
//    case AC_Control_Code::AC_SET_TEMPERATURE_RESULT:
//        if((uint32_t)outBufferSize >= sizeof(int)) {
//            *((int*)outBuffer) = cmd_result_;
//            bytesReturned = sizeof(int);
//        } else {
//            bytesReturned = 0;
//        }
//        break;
//    default:
//        if((uint32_t)outBufferSize >= sizeof(int)) {
//            *((int*)outBuffer) = 2;//无效命令
//            bytesReturned = sizeof(int);
//        } else {
//            bytesReturned = 0;
//        }
//        break;
//    }
//    return 0;
}

void HBZS96K::RunCheckThreshold()
{
	//CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回路", cData.A_line_incoming_fuse, 1);
  //CheckThresholdBool(1, "0777001", "0777001", "状态告警", "回路", cData.A_circuit_lightning_arrester>>5&0x1, 1);
  CheckThresholdBool(2, "0777001", "0777001", "A路进线熔断器", "A路进线熔断器", cData.A_line_incoming_fuse == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "A路防雷器", "A路防雷器", cData.A_circuit_lightning_arrester == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "状态告警", "B路防雷器", cData.B_circuit_lightning_arrester == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "B路防雷器", "A路进线电压", cData.A_line_incoming_voltage == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "A路进线电流", "A路进线电流", cData.A_line_incoming_current == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "A路进线功率", "A路进线功率", cData.A_line_incoming_power == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "B路进线熔断器", "B路进线熔断器", cData.B_circuit_lightning_arrester == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "B路进线电压", "B路进线电压", cData.B_line_incoming_voltage == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "B路进线电流", "B路进线电流", cData.B_line_incoming_current == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "B路进线功率", "B路进线功率", cData.B_line_incoming_power == 1, signal_index_++);
  CheckThresholdBool(2, "0777001", "0777001", "支路熔丝告警", "支路熔丝告警", cData.Branch_circuit_fuse_alarm == 1, signal_index_++);
}



float HBZS96K::Get_Value ( uint32_t data_id, const std::string& var_name ) const
{
    if ( !bIsDataReady_ )
        throw std::out_of_range ( "数据未就绪" );
    throw std::out_of_range ( "不支持变量" );
}

#ifdef USE_SEPERATE_DRIVER


extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back ( std::make_shared<HBZS96KProvider>() );
    return std::move ( providerVec );
}

#endif
