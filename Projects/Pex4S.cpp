#include "Pex4S.h"
#include "UniDataDevice.cpp"


Pex4S::Pex4S()
{
    device_type_ = "pex4s";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

Pex4S::~Pex4S()
{
}


bool Pex4S::InitSetting(const Json::Value &settingRoot)
{
	cData.data_id = data_id_;
	if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
        	if(settingRoot["appSetting"]["ct"] != Json::nullValue) {
            		ct_ = atoi(settingRoot["appSetting"]["ct"].asString().c_str());
        	}
        	if(settingRoot["appSetting"]["has_a_pex"] != Json::nullValue) {
            		has_a_pex_ = atoi(settingRoot["appSetting"]["has_a_pex"].asString().c_str());
        	}
	}
	return UniDataDevice<Pex4S_Data_t, SPModbus, RT_Pex4S>::InitSetting(settingRoot);
}

void Pex4S::RunCheckThreshold()
{
	//alarm1
	CheckThresholdBool(SMDController::LEVEL1, "alarm_1_0", "70601", "报警状态1", "远程关机报警", (cData.alarm1 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_1", "70601", "报警状态1", "地板溢水报警", ((cData.alarm1 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_2", "70601", "报警状态1", "烟感报警", ((cData.alarm1 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_3", "70601", "报警状态1", "火感报警", ((cData.alarm1 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_4", "70601", "报警状态1", "防雷器报警", ((cData.alarm1 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_5", "70601", "报警状态1", "自定义1报警", ((cData.alarm1 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_6", "70601", "报警状态1", "自定义2报警", ((cData.alarm1 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_7", "70601", "报警状态1", "自定义3报警", ((cData.alarm1 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_8", "70601", "报警状态1", "自定义4报警", ((cData.alarm1 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_9", "70601", "报警状态1", "自定义5报警", ((cData.alarm1 >> 9) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_10", "70601", "报警状态1", "自定义6报警", ((cData.alarm1 >> 10) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_11", "70601", "报警状态1", "消防报警", ((cData.alarm1 >> 11) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_12", "70601", "报警状态1", "水盘超高水位报警", ((cData.alarm1 >> 12) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_13", "70601", "报警状态1", "注水故障报警", ((cData.alarm1 >> 13) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_1_14", "70601", "报警状态1", "水位开关故障报警", ((cData.alarm1 >> 14) & 0x1), signal_index_++);
    //alarm2
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_0", "70601", "报警状态2", "电源丢失报警", (cData.alarm2 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_1", "70601", "报警状态2", "电源过压报警", ((cData.alarm2 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_2", "70601", "报警状态2", "电源欠压报警", ((cData.alarm2 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_3", "70601", "报警状态2", "电源频偏报警", ((cData.alarm2 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_4", "70601", "报警状态2", "电源缺相报警", ((cData.alarm2 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_5", "70601", "报警状态2", "电源反相报警", ((cData.alarm2 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_6", "70601", "报警状态2", "交流断电报警", ((cData.alarm2 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_7", "70601", "报警状态2", "直流断电报警", ((cData.alarm2 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_8", "70601", "报警状态2", "联动地址重复报警", ((cData.alarm2 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_9", "70601", "报警状态2", "联动从机丢失报警", ((cData.alarm2 >> 9) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_2_10", "70601", "报警状态2", "湿膜维护提醒", ((cData.alarm2 >> 10) & 0x1), signal_index_++);
   //alarm3
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "风机维护提醒", cData.alarm3&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "过滤网维护提醒", (cData.alarm3>>1)&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "电加热维护提醒", (cData.alarm3>>2)&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "加湿器维护提醒", (cData.alarm3>>3)&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "压缩机维护提醒", (cData.alarm3>>4)&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3#include "SPModbus.h"", "风机故障报警", (cData.alarm3>>5)&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "电加热故障报警", (cData.alarm13>>6)&0x1, signal_index_++);
   CheckThresholdBool(SMDController::LEVEL1, "0777001", "0777001", "报警状态3", "加湿器故障报警", (cData.alarm3>>7)&0x1, signal_index_++);
   
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_8", "70601", "报警状态3", "红外加湿高水位报警", ((cData.alarm3 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_9", "70601", "报警状态3", "过滤网堵塞报警", ((cData.alarm3 >> 9) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_10", "70601", "报警状态3", "风阀故障报警", ((cData.alarm3 >> 10) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_11", "70601", "报警状态3", "气流丢失报警", ((cData.alarm3 >> 11) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_12", "70601", "报警状态3", "冷凝高水位报警", ((cData.alarm3 >> 12) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_13", "70601", "报警状态3", "冷凝水泵报警", ((cData.alarm3 >> 13) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_14", "70601", "报警状态3", "水阀故障报警", ((cData.alarm3 >> 14) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_3_15", "70601", "报警状态3", "水阀维护提醒", ((cData.alarm3 >> 15) & 0x1), signal_index_++);
    //alarm4
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_0", "70601", "报警状态4", "回风高温报警", (cData.alarm4 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_1", "70601", "报警状态4", "回风低温报警", ((cData.alarm4 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_2", "70601", "报警状态4", "送风高温报警", ((cData.alarm4 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_3", "70601", "报警状态4", "送风低温报警", ((cData.alarm4 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_4", "70601", "报警状态4", "远程高温报警", ((cData.alarm4 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_5", "70601", "报警状态4", "远程低温报警", ((cData.alarm4 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_6", "70601", "报警状态4", "回风高湿报警", ((cData.alarm4 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_7", "70601", "报警状态4", " 回风地湿报警", ((cData.alarm4 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_8", "70601", "报警状态4", "送风高湿报警", ((cData.alarm4 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL4, "alarm_4_9", "70601", "报警状态4", "送风地湿报警", ((cData.alarm4 >> 9) & 0x1), signal_index_++);
	 CheckThresholdBool(SMDController::LEVEL1, "alarm_4_10", "70601", "报警状态4", "远程高湿报警", ((cData.alarm4 >> 10) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_4_11", "70601", "报警状态4", "远程地湿报警", ((cData.alarm4 >> 11) & 0x1), signal_index_++);
	//alarm5
	CheckThresholdBool(SMDController::LEVEL1, "alarm_5_0", "70601", "报警状态5", "回风温度传感器故障", (cData.alarm5 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_1", "70601", "报警状态5", "回风湿度传感器故障", ((cData.alarm5 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_2", "70601", "报警状态5", "送风温度传感器故障", ((cData.alarm5 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_3", "70601", "报警状态5", "送风湿度传感器故障", ((cData.alarm5 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_4", "70601", "报警状态5", "远程温度传感器故障", ((cData.alarm5 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_5", "70601", "报警状态5", "远程湿度传感器故障", ((cData.alarm5 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_6", "70601", "报警状态5", "回风传感器通讯故障", ((cData.alarm5 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_7", "70601", "报警状态5", "送风传感器通讯故障", ((cData.alarm5 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_5_8", "70601", "报警状态5", "远程传感器通讯故障", ((cData.alarm5 >> 8) & 0x1), signal_index_++);
	//alarm6
	CheckThresholdBool(SMDController::LEVEL1, "alarm_6_0", "70601", "报警状态6", "气流丢失传感器故障", (cData.alarm6 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_1", "70601", "报警状态6", "高压压力传感器故障", ((cData.alarm6 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_2", "70601", "报警状态6", "低压压力传感器故障", ((cData.alarm6 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_3", "70601", "报警状态6", "排气温度传感器故障", ((cData.alarm6 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_4", "70601", "报警状态6", "吸气温度传感器故障", ((cData.alarm6 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_5", "70601", "报警状态6", "进水温度传感器故障", ((cData.alarm6 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_6", "70601", "报警状态6", "出水温度传感器故障", ((cData.alarm6 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_7", "70601", "报警状态6", "风压传感器故障", ((cData.alarm6 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_8", "70601", "报警状态6", "水压传感器故障", ((cData.alarm6 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_9", "70601", "报警状态6", "水流量传感器故障", ((cData.alarm6 >> 9) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_10", "70601", "报警状态6", "滤网压差传感器故障", ((cData.alarm6 >> 10) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_6_11", "70601", "报警状态6", "低压压力传感器故障锁定", ((cData.alarm6 >> 11) & 0x1), signal_index_++);
	//alarm7
	CheckThresholdBool(SMDController::LEVEL1, "alarm_7_0", "70601", "报警状态7", "高压报警", (cData.alarm7 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_1", "70601", "报警状态7", "高压锁定报警", ((cData.alarm7 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_2", "70601", "报警状态7", "低压报警", ((cData.alarm7 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_3", "70601", "报警状态7", "低压锁定报警", ((cData.alarm7 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_4", "70601", "报警状态7", "排气高温报警", ((cData.alarm7 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL4, "alarm_7_5", "70601", "报警状态7", "排气高温锁定报警", ((cData.alarm7 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL4, "alarm_7_6", "70601", "报警状态7", "排气低温报警", ((cData.alarm7 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL4, "alarm_7_7", "70601", "报警状态7", "排气低温锁定报警", ((cData.alarm7 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_8", "70601", "报警状态7", "排气过热度低报警", ((cData.alarm7 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_9", "70601", "报警状态7", "排气过热度低锁定报警", ((cData.alarm7 >> 9) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_10", "70601", "报警状态7", "盘管冻结报警", ((cData.alarm7 >> 10) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_11", "70601", "报警状态7", "盘管冻结锁定报警", ((cData.alarm7 >> 11) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_12", "70601", "报警状态7", "压缩机高压异常报警", ((cData.alarm7 >> 12) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_13", "70601", "报警状态7", "压差报警", ((cData.alarm7 >> 13) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_14", "70601", "报警状态7", "压差锁定报警", ((cData.alarm7 >> 14) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_7_15", "70601", "报警状态7", "压缩机低压异常报警", ((cData.alarm7 >> 15) & 0x1), signal_index_++);
	//alarm8
	CheckThresholdBool(SMDController::LEVEL1, "alarm_8_0", "70601", "报警状态8", "电子膨胀阀驱动通信故障", (cData.alarm8 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_1", "70601", "报警状态8", "电子膨胀阀驱动故障", ((cData.alarm8 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_2", "70601", "报警状态8", "压缩机驱动通信故障", ((cData.alarm8 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_3", "70601", "报警状态8", "压缩机驱动故障", ((cData.alarm8 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_4", "70601", "报警状态8", "压缩机驱动故障锁定", ((cData.alarm8 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_5", "70601", "报警状态8", "风机检测板通讯故障", ((cData.alarm8 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_6", "70601", "报警状态8", "压缩机驱动通信锁定", ((cData.alarm8 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_7", "70601", "报警状态8", "功率表通讯故障", ((cData.alarm8 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_8", "70601", "报警状态8", "制冷剂泵通讯故障", ((cData.alarm8 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_9", "70601", "报警状态8", "冷凝器通讯故障", ((cData.alarm8 >> 9) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_8_10", "70601", "报警状态8", "节能卡故障", ((cData.alarm8 >> 10) & 0x1), signal_index_++);
	//alarm9
	CheckThresholdBool(SMDController::LEVEL1, "alarm_9_0", "70601", "报警状态9", "群控主机丢失报警", (cData.alarm9 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_9_1", "70601", "报警状态9", "群控从机丢失报警", ((cData.alarm9 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_9_2", "70601", "报警状态9", "群控地址重复报警", ((cData.alarm9 >> 2) & 0x1), signal_index_++);
	//alarm10
	CheckThresholdBool(SMDController::LEVEL1, "alarm_10_0", "70601", "报警状态10", "进水高温报警", (cData.alarm10 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_1", "70601", "报警状态10", "进水低温报警", ((cData.alarm10 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_2", "70601", "报警状态10", "出水高温报警", ((cData.alarm10 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_3", "70601", "报警状态10", "出水低温报警", ((cData.alarm10 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_4", "70601", "报警状态10", "进水高压报警", ((cData.alarm10 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_5", "70601", "报警状态10", "进水低压报警", ((cData.alarm10 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_6", "70601", "报警状态10", "水流丢失报警", ((cData.alarm10 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_10_7", "70601", "报警状态10", "水流量低报警", ((cData.alarm10 >> 7) & 0x1), signal_index_++);
	//alarm11
	CheckThresholdBool(SMDController::LEVEL1, "alarm_11_0", "70601", "报警状态11", "制冷剂泵驱动故障", (cData.alarm11 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_1", "70601", "报警状态11", "制冷剂泵入口压力传感器故障", ((cData.alarm11 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_2", "70601", "报警状态11", "制冷剂泵出口压力传感器故障", ((cData.alarm11 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_3", "70601", "报警状态11", "制冷剂泵环境温度故障", ((cData.alarm11 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_4", "70601", "报警状态11", "制冷剂泵过温故障", ((cData.alarm11 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_5", "70601", "报警状态11", "制冷剂泵过温锁定故障", ((cData.alarm11 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_6", "70601", "报警状态11", "制冷剂泵高扬程告警", ((cData.alarm11 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_7", "70601", "报警状态11", "制冷剂泵高扬程锁定告警", ((cData.alarm11 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_8", "70601", "报警状态11", "制冷剂泵低扬程告警", ((cData.alarm11 >> 8) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_11_9", "70601", "报警状态11", "制冷剂泵低扬程锁定告警", ((cData.alarm11 >> 9) & 0x1), signal_index_++);
	//alarm12
	CheckThresholdBool(SMDController::LEVEL1, "alarm_12_0", "70601", "报警状态12", "冷凝器驱动故障", (cData.alarm12 & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_1", "70601", "报警状态12", "冷凝器压力传感器故障", ((cData.alarm12 >> 1) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_2", "70601", "报警状态12", "冷凝器风机故障", ((cData.alarm12 >> 2) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_3", "70601", "报警状态12", "冷凝器风机故障锁定", ((cData.alarm12 >> 3) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_4", "70601", "报警状态12", "冷凝器喷淋高压故障", ((cData.alarm12 >> 4) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_5", "70601", "报警状态12", "冷凝器低压差故障", ((cData.alarm12 >> 5) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_6", "70601", "报警状态12", "冷凝器环境温度传感器故障", ((cData.alarm12 >> 6) & 0x1), signal_index_++);
    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_7", "70601", "报警状态12", "冷凝器风机未配置", ((cData.alarm12 >> 7) & 0x1), signal_index_++);

    CheckThresholdBool(SMDController::LEVEL1, "alarm_12_8", "70601", "报警状态12", "冷凝器喷淋水系统故障", ((cData.alarm12 >> 8) & 0x1), signal_index_++);
}

bool Pex4S::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = Pex4S_R3_20;
    modbus_read_registers(20, 31);
    return true;
}

bool Pex4S::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case Pex4S_R3_20:{
		  cData.uint_status = tab_reg[0];
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
		  cData.
            memcpy(cData.r3_20, tab_reg, sizeof(uint16_t)*31);
            state = Pex4S_R3_21;
            modbus_read_registers(21, 1);
	    break;
      }
      case Pex4S_R3_21:{
            memcpy(cData.r3_21, tab_reg, sizeof(uint16_t)*1);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float Pex4S::Get_Value(uint32_t data_id, const std::string& var_name) const
{
    if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }

   if(var_name == "机组状态"){
     return cData.r3_20[0];
   }
   else if(var_name == "运行状态"){
     return cData.r3_20[1];
   }
    throw std::out_of_range("不支持变量");
}



int Pex4S::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    providerVec.push_back(std::make_shared<Pex4SProvider>());
    return std::move(providerVec);
}

#endif
