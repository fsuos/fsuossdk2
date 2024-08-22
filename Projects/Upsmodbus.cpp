#include "Upsmodbus.h"
#include "UniDataDevice.cpp"


Upsmodbus::Upsmodbus()
{
    device_type_ = "upsmodbus";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

Upsmodbus::~Upsmodbus()
{
}



void Upsmodbus::RunCheckThreshold()
{
	//alarm1
    CheckThresholdBool(1, "0777001", "0777001", "状态告警", "设备通信异常", cData.alarm1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "逆变器不同步", cData.alarm1>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "主路电压异常", cData.alarm1>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "整流器故障", cData.alarm1>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "逆变器故障", cData.alarm1>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路异常", cData.alarm1>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池状态", cData.alarm1>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "环境过温", cData.alarm1>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "PowerCAN通讯异常", cData.alarm1>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池放电限流", cData.alarm1>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "放电器故障", cData.alarm1>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "充电器故障", cData.alarm1>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "LBS连接线异常", cData.alarm1>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池需维护", cData.alarm1>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池过温", cData.alarm1>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池接地故障", cData.alarm1>>15&0x1, 1);
//alarm2
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "主路频率异常", cData.alarm2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "模块过温", cData.alarm2>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输入变压器过温", cData.alarm2>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路柜变压器过温", cData.alarm2>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "均充超时", cData.alarm2>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "辅助电源掉电", cData.alarm2>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "主路相序反", cData.alarm2>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "整流器软启动失败", cData.alarm2>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "用户操作错误", cData.alarm2>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出过载", cData.alarm2>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "并机系统容量过载", cData.alarm2>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出过载超时", cData.alarm2>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "负载冲击转旁路", cData.alarm2>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "并机均流异常", cData.alarm2>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "母线异常关机", cData.alarm2>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "邻机请求转旁路", cData.alarm2>>15&0x1, 1);
//alarm3
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "直流母线过压", cData.alarm3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "MonCAN通讯异常", cData.alarm3>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输入缺零故障", cData.alarm3>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "逆变继电器故障", cData.alarm3>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "主路反灌", cData.alarm3>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输入电流异常", cData.alarm3>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出电压异常", cData.alarm3>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输入电流超限 ", cData.alarm3>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路过温", cData.alarm3>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "邻机旁路晶闸管故障", cData.alarm3>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "并机连接线异常", cData.alarm3>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "充电器过温", cData.alarm3>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池端口异常", cData.alarm3>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池房环境异常", cData.alarm3>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "风扇异常", cData.alarm3>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路接管", cData.alarm3>>15&0x1, 1);
//alarm4
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "BCB1状态异常", cData.alarm4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "BCB2状态异常", cData.alarm4>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "BCB3状态异常", cData.alarm4>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "BCB4状态异常", cData.alarm4>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路模块未授权", cData.alarm4>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "功率模块未授权", cData.alarm4>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "监控未授权", cData.alarm4>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路SCR风扇异常", cData.alarm4>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "平衡电路故障", cData.alarm4>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "平衡电路过流", cData.alarm4>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "平衡电路过温", cData.alarm4>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "失去冗余", cData.alarm4>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "功率板设置不匹配", cData.alarm4>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "模块未就绪", cData.alarm4>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "上抽风风扇异常", cData.alarm4>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "系统A相输出熔丝断", cData.alarm4>>15&0x1, 1);
//alarm5
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "系统B相输出熔丝断", cData.alarm5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "系统C相输出熔丝断", cData.alarm5>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "参数配置失败", cData.alarm5>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路过流超时", cData.alarm5>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "零地过压", cData.alarm5>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路无效", cData.alarm5>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出变压器过温", cData.alarm5>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出熔丝断", cData.alarm5>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池自检失败", cData.alarm5>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "分级下电1生效", cData.alarm5>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "分级下电2生效", cData.alarm5>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路功率模块未就绪", cData.alarm5>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路控制模块未就绪", cData.alarm5>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "系统主路熔丝断", cData.alarm5>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "旁路熔丝断", cData.alarm5>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "系统输出熔丝断", cData.alarm5>>15&0x1, 1);
//alarm6
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "维修旁路供电", cData.alarm6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出短路", cData.alarm6>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池深度放电", cData.alarm6>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "双旁路供电", cData.alarm6>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池温度异常", cData.alarm6>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "主路熔丝断", cData.alarm6>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "放电器关闭", cData.alarm6>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "ParaMonCAN通讯异常", cData.alarm6>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "BMS通讯异常", cData.alarm6>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "锂电池需脱扣", cData.alarm6>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "电池系统异常", cData.alarm6>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "输出变压器风扇异常", cData.alarm6>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "内部通信状态", cData.alarm6>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "防雷器1开关告警", cData.alarm6>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "防雷器1告警", cData.alarm6>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "防雷器2开关告警", cData.alarm6>>15&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "防雷器2告警", cData.alarm7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "SPM CRC校验错误", cData.alarm7>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板1就绪", cData.alarm7>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板2就绪", cData.alarm7>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板3就绪", cData.alarm7>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板4就绪", cData.alarm7>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板5就绪", cData.alarm7>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板6就绪", cData.alarm7>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板7就绪", cData.alarm7>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板8就绪", cData.alarm7>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板9就绪", cData.alarm7>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板10就绪", cData.alarm7>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板11就绪", cData.alarm7>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "采集板12就绪", cData.alarm7>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1过压", cData.alarm7>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1欠压", cData.alarm7>>15&0x1, 1);
//alarm8
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1缺相", cData.alarm8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1掉电", cData.alarm8>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1电流超高阈值", cData.alarm8>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1电流超低阈值", cData.alarm8>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1冲击过流", cData.alarm8>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源1频率异常", cData.alarm8>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2过压", cData.alarm8>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2欠压", cData.alarm8>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2缺相", cData.alarm8>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2掉电", cData.alarm8>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2电流超高阈值", cData.alarm8>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2电流超低阈值", cData.alarm8>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2冲击过流", cData.alarm8>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源2频率异常", cData.alarm8>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出过压", cData.alarm8>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出欠压", cData.alarm8>>15&0x1, 1);
//alarm9
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出缺相", cData.alarm9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出掉电", cData.alarm9>>1&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出电流超高阈值", cData.alarm9>>2&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出电流超低阈值", cData.alarm9>>3&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出冲击过流", cData.alarm9>>4&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "源输出频率异常", cData.alarm9>>5&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路1电流过流", cData.alarm9>>6&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路1电流超高阈值", cData.alarm9>>7&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路1电流超低阈值", cData.alarm9>>8&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路1电流冲击过流", cData.alarm9>>9&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路1开关异常", cData.alarm9>>10&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路72电流过流", cData.alarm9>>11&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路72电流超高阈值", cData.alarm9>>12&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路72电流超低阈值", cData.alarm9>>13&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路72电流冲击过流", cData.alarm9>>14&0x1, 1);
CheckThresholdBool(1, "0777001", "0777001", "状态告警", "支路72开关异常", cData.alarm9>>15&0x1, 1);

}

bool Upsmodbus::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = Upsmodbus_R3_0;
    modbus_read_registers(0, 105);
    return true;
}

bool Upsmodbus::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case Upsmodbus_R3_0:{
            memcpy(cData.r3_0, tab_reg, sizeof(uint16_t)*105);
            state = Upsmodbus_R3_106;
            modbus_read_registers(106, 106);
	    break;
      }
      case Upsmodbus_R3_106:{
            memcpy(cData.r3_106, tab_reg, sizeof(uint16_t)*106);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float Upsmodbus::Get_Value(uint32_t data_id, const std::string& var_name) const
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
    providerVec.push_back(std::make_shared<UpsmodbusProvider>());
    return std::move(providerVec);
}

#endif
