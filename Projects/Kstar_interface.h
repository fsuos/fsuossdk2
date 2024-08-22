#ifndef  KSTAR_INTERFACE_H
#define  KSTAR_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


//UPS 实时数据 G1
struct CMD_G1_t
{
 uint16_t input_valtage; //输入电压
 uint16_t output_valtage;//输出电压      
 uint16_t Inverted_voltage ; //逆变电压
 uint16_t positive_battery_voltage ;     //正电池电压       
 uint16_t negative_battery_voltage ; //负电池电压
 uint16_t input_frequency; //输入频率
 uint16_t load_rate;    ///负载率
 uint16_t ectification_temperature ;  //整流温度
 uint16_t positive_bus_voltage; //正母线电压
 uint16_t negative_bus_voltage ;//负母线电压
 uint16_t Input_phase_A_point;//输入A相电压
 uint16_t  Input_frequency ;//输入频率
 uint16_t Input_B-phase_voltage;//输入B相电压
 uint16_t Input_C-phase_voltage;//输入C相电压
 uint16_t output_voltage;//输出电压
 uint16_t output_frequency;//输出频率
 uint16_t output_current;//输出电流
 uint16_t 	output_power_factor;//输出功因
 uint16_t peak_load_ratio ;//负载峰值比
	uint16_t 	bypass_point ;//旁路电压
 uint16_t bypass_frequency;//旁路频率
 uint16_t positive_BUS_voltage;//正BUS电压
 uint16_t negative_BUS_voltage;//负BUS电压
 uint16_t positive_battery_pack_voltage ;//正电池组电压
 uint16_t negative_battery_pack_voltage ;//负电池组电压
 uint16_t positive_battery_pack_current ;//正电池组电流
 uint16_t negative_battery_pack_current ;//负电池组电流
 uint16_t bus_capacitor_operating_time ;//母线电容运行时间
 uint16_t number_of_parallel_machines_on_board ;//并机上台数
 uint16_t number_of_parallel_inverter_output_units ;//并机逆变输出台数
 uint16_t internal_temperature ;//机内温度
  uint16_t rectification_temperature ;//整流温度
 uint16_t inversion_temperature ;//逆变温度
 uint16_t inverted_voltage ;//逆变电压
 uint16_t inversion_frequency ;//逆变频率
 uint16_t positive_battery_capacity_rate ;//正组电池容量率
 uint16_t positive_battery_backup_time ;//正组电池后备时间
 uint16_t negative_battery_capacity_rate ;//负组电池容量率
 uint16_t negative_battery_capacity_rate ;//负组电池后备时间
 uint16_t negative_battery_capacity_rate ;//有功功率
 uint16_t apparent_power ;//视在功率
  uint16_t cclass_alarm_information_internal_code;//c类报警信息内码
uint16_t  input A-phase current;//输入A相电流
 uint16_t input B-phase current;//输入B相电流
 uint16_t input_C-phase_current ;//输入C相电流
 uint16_t input_power_factor ;//输入功因
};


//UPS状态查询G2
struct CMD_G2_t
{
    char reserved_1;                //7 未使用0
    char citySupplyRevert;          //6 1:市电相序反
    char batteryVoltageLow;         //5 1:电池电压低,关逆变(EOD)
    char batteryVoltageLowAlarm;    //4 1:电池电压低告警
    char reserved_2;                //3 未使用
    char citySupplynormal;          //2 0:市电输入正常
    char chargeStatus;              //1 1:电池均充中 0:电池浮充
    char SMRStatus;                 //0 1:整流器运行中

    char reserved_3;                //7 未使用0
    char reserved_4;                //6 未使用0
    char reserved_5;                //5 未使用0
    char bypassFre;                 //4 1:旁路频率异常
    char reserved_6;                //3 未使用0
    char BypassInputNormal;         //2 1:旁路输入正常 0:旁路输入不正常
    char contravariantOuput;        //1 1:逆变输出 0:旁路输出
    char revertRun;                 //0 1:逆变器运行中

    char reserved_7;                //7 未使用0
    char SMR_breakDown;             //6 1:整流器故障
    char reserved_8;                //5 未使用0
    char reserved_9;                //4 未使用0
    char payOver;                   //3 1:过载
    char reverterOutputAbnormal;    //2 1:逆变器输出不正常
    char overtemperature;           //1 1:过温
    char reserved_10;               //0 未使用0
};


//UPS 三相实时数据 G3
struct CMD_G3_t
{
    char ThreePhaseInputVoltage[17];       //b.三相输入电压:NNN.N/NNN.N/NNN.N,N是一个范围从0~9的整数,单位是V
    char ThreeBypassInputVoltage[17];      //c.三相旁路电压:PPP.P/PPP.P/PPP.P,P是一个范围从0~9的整数,单位是V
    char OutputVoltage[17];                //d.输出电压:QQQ.Q/QQQ.Q/QQQ.Q,Q是一个范围从0~9的整数,单位是V,前面两项数据未使用
    char Payloadpercent[17];               //e.负载百分数:SSS.S/SSS.S/SSS.S,S是一个范围从0~9的整数,单位是百分之,前面两项数据未使用
};


//UPS 信息命令 I
struct CMD_I_t
{
    char name[15];                  //公司名称:15 个字母(字节),如果不够则用空格填满
    char model[10];                 //UPS 型号:10 个字母(字节),如果不够则用空格填满
    char ver[10];                   //版本:10个字母(字节),如果不够则用空格填满,每一项之间有空格
};

//UPS 额定信息 GF
struct CMD_GF_t
{
    char SMRPhaseVoltage[14];           //a.整流器相电压、线电压(Rect_Volt)14个字节,如果数据不够多则用空格填满,例如:220V 3P3W,220V/380V 3P4W,3P代表三相,3W代表三线,3P4W为三相四线制
    char SMRFre[3];                     //b.整流器频率 CCC
    char BypassPhaseVoltage[14];        //c.旁路相电压、线电压(Bpss_Volt),14个字节,如果数据不够多则用空格填满,例如:220V 3P3W,220V/380V 3P4W
    char BypassFre;                     //d.旁路频率 FFF
    char outputVoltage[14];             //e.输出相电压、线电压(O/P_Volt),14个字节,如果数据不够多则用空格填满,例如:220V 3P3W,220V/380V 3P4W
    char specifiedOutputFre[3];         //f.额定输出频率 QQQ
    char specifiedbatteryVoltage;       //g.额定电池电压 SSS
    char specifiedpower;                //h. 额定功率,10 个字节,如果数据不够多则用空格填满
};

//状态查询 Q1
struct CMD_Q1_t
{
    char inputVoltage[5];           //b.输入电压:MMM.M,M是一个范围从0~9的整数,单位是V
    char reserved_1[5];             //c.未使用:000.0
    char outputVoltage[5];          //d.输出电压:PPP.P,P是一个范围从0~9的整数,单位是V
    char outputCurrent[3];          //e.输出电流:QQQ,QQQ是相对最大电流的百分比,不是绝对值
    char intputFre[4];              //f.输入频率RR.R,R是一个范围从0~9的整数,单位是’Hz’
    char batterySingleVoltage[4];   //g.电池单体电压:S.SS,为电池每节实际电压的1/6.
    char temperature[4];            //h.温度:TT.T

    char inputCitySuppltNormal;     //7 0:输入市电正常
    char batteryLow;                //6 1:电池低压
    char BypassOutput;              //5 1:旁路输出
    char reserved_2;                //4 未使用0
    char UPSOnlineModel;            //3 UPS为在线式0
    char batteryTest;               //2 1：电池测试中
    char shutdown;                  //1 1：关闭，旁路输出
    char reserved;                  //0 未使用
};


//UPS 额定信息 F
struct CMD_F_t
{
    char specifiedVoltage[5];       //a.额定电压 MMM.M,单位v
    char specifiedCurrent[3];       //b.额定电流 QQQ,单位A
    char batteryVoltage[5];         //c.电池电压 SSS.S,单位 v
    char fre[4];                    //d.频率 RR.R,单位 Hz 1
};

struct EA86_t
{
    int data_id;
    CMD_G1_t CMD_G1;
    CMD_G2_t CMD_G2;
    CMD_G3_t CMD_G3;
    CMD_I_t CMD_I;
    CMD_GF_t CMD_GF;
    CMD_Q1_t CMD_Q1;
    CMD_F_t CMD_F;
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
