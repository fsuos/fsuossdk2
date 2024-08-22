#ifndef HBZS96K_INTERFACE_H
#define HBZS96K_INTERFACE_H


#include "common_interface.h"
#pragma pack(push)
#pragma pack(1)

struct HBZS96K_data_t {
    unsigned int data_id;
    //获取模拟量参数 --60_42
    float  DC VoltageA ; //A路直流电压
    float  DC currentA ; //A路直流电流
    float DC powerA; //A路直流功率
    //获取开关机状态 --60_43
    //unsigned char air_state; //空调状态
    //获取系统参数 --60_47
    float DC energyA; //A路直流电能
    float DC current demandA; //A路直流电流需量
    float DC power demandA; //A路直流功率需量
    float DC VoltageB ; //B路直流电压
    float DC currentB; //B路直流电流
    float DC powerB; //B路直流功率
	float DC energyB; //B路直流电能
	float DC current demandB;//B路直流电流需量
	float DC power demandB;//B路直流功率需量
//  //获取系统监测模块时间 --60_4D
//  unsigned short sys_year;
//  unsigned char sys_month;
//  unsigned char sys_day;
//  unsigned char sys_hour;
//  unsigned char sys_minute;
//  unsigned char sys_seconds;
    //获取机组状态 --60_82
    unsigned char number_of_circuits; //回路数
    unsigned char  statusA; //A路1-72路支路开关状态
    unsigned char statusB; //B路1-72路支路开关状态
//    unsigned char heat_status; //加热状态
//    unsigned char humid_status; //加湿状态
//    unsigned char dehumid_status; //除湿状态
    //unsigned char alert_status; //告警状态
    unsigned char A_line_incoming_fuse; //A路进线熔断器
    unsigned char A_circuit_lightning_arrester; //A路防雷器
    unsigned char B_circuit_lightning_arrester; //B路防雷器
    unsigned char A_line_incoming_voltage; //A路进线电压
    unsigned char A_line_incoming_current; //A路进线电流
    unsigned char A_line_incoming_power; //A路进线功率
    unsigned char B_circuit_lightning_arrester; //B路进线熔断器
    unsigned char B_line_incoming_voltage;  //B路进线电压
    unsigned char B_line_incoming_current; //B路进线电流
    unsigned char B_line_incoming_power; //B路进线功率
    unsigned char Branch_circuit_fuse_alarm; //支路熔断告警
//    unsigned char humid_mainten_alarm; //加湿器维护报警
//    unsigned char filter_mainten_alarm; //过滤网维护报警
//    unsigned char com_failer_alarm; //通讯故障报警
//    unsigned char coil_freeze_alarm; //盘管冻结告警
//    unsigned char humid_fault_alarm; //加湿器故障报警
//    unsigned char sensor_miss_alarm; //传感器板丢失告警
//    unsigned char gas_temp_fault_alarm; //排气温度故障告警
//    unsigned char power_miss_fault_alarm; //电源丢失故障告警
//    unsigned char power_undervol_alarm; //电源过欠压报警
//    unsigned char power_phase_alarm; //电源缺相报警
//    unsigned char power_freq_alarm; //电源频率偏移报警
//    unsigned char floor_overflow_alarm; //地板溢水报警
//    unsigned char save_card_failure; //节能卡故障

    //获取当前机组状态  --60_83
//    unsigned char engine_status; //机组状态
//    unsigned char lock_status; //高压锁定
    tele_c_time update_time;
};

#pragma pack(pop)

#endif
