#ifndef Pex4S_INTERFACE_H
#define Pex4S_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct Pex4S_Data_t {
    unsigned int data_id;
    uint16_t r3_20[31];
    uint16_t r3_21[1];
	uint16_t alarm1,alarm2,alarm3,alarm4,alarm5,alarm6,alarm7,alarm8,alarm9,alarm10,alarm11,alarm12;//告警状态1～12
	uint16_t uint_status;//机组状态
	uint16_t control_status;//群控状态
	uint16_t run_status;//运行状态
	uint16_t Ua;//A相电压
	uint16_t Ub;//B相电压
	uint16_t Uc;//C相电压
	uint16_t P;//电源频率
	uint16_t return_temperature1,return_temperature2,return_temperature3//回风温度1 2 3  
	uint16_t return_humidity1, return_humidity2, renturn_humidity3;//回风湿度1 2 3
	uint16_t supply1_temperature1, supply1_temperature2, supply1_temperature3;//送风温度1 2 3   系统1
	uint16_t supply1_humidity, supply1_humidity, supply1_humidity;//送风湿度1 2 3    系统1
	uint16_t remote_temperature1,remote_temperature2,remote_temperature3,remote_temperature4,
					   remote_temperature5,remote_temperature6,remote_temperature7,remote_temperature8,
					    remote_temperature9,remote_temperature10,remote_temperature11,remote_temperature12,
						remote_temperature13,remote_temperature14,remote_temperature15,remote_temperature16;//远程温度1～16
	
	uint16_t remote_humidity1,remote_humidity2,remote_humidity3,remote_humidity4,
						remote_humidity5,remote_humidity6,remote_humidity7,remote_humidity8,
						remote_humidity9,remote_humidity10,remote_humidity11,remote_humidity12,
						remote_humidity13,remote_humidity14,remote_humidity15,remote_humidity16;//远程湿度1～16
						
	uint16_t in_temperatureA;//A路进水温度
	uint16_t out_temperatureA;//A路出水温度
	uint16_t in_temperatureB;//B路进水温度
	uint16_t out_temperatureB;//B路出水温度
	uint16_t in_pressureA;//A路进水压力
	uint16_t out_pressureA;//A路出水压力
	uint16_t in_pressureB;//B路进水压力
	uint16_t out_pressureB;//B路出水压力
	uint16_t dischargeA;//A路水流量
	uint16_t dischargeB;//B路水流量
	uint16_t wind_pressure1;//风压1
	uint16_t wind_pressure2;//风压2
	uint16_t wind_speed1;//风机转速1
	uint16_t wind_speed2//风机转速2
	uint16_t compressor_capacity1;//压缩机1容量
	uint16_t compressor_capacity2;//压缩机2容量
	uint16_t  Water_valve_opening1;//水阀1开度
	uint16_t  Water_valve_opening1;//水阀2开度
	uint16_t monitor;//监控开关机  开机：0x10   关机：0x1F
	uint16_t return_temperature_setting;//回风温度设定
	uint16_t supply_temperature_setting;//送风温度设定
	uint16_t remote_temperature_setting;//远程温度设定
	uint16_t humidity_setting;//湿度设定
	uint16_t temperature_proportional_band;//温度比例带
	uint16_t humidity_propotional_band;//湿度比例带
	uint16_t return_humidity_setting;//回风湿度设定
	uint16_t romote_humidity_setting;//远程湿度设定
	uint16_t fan_temperature_difference_setting;//风机温差设定
	uint16_t humidity_control_method;//湿度控制方式
	uint16_t fan_control_mode;//风机控制方式
	uint16_t compressor_control_mode;//压缩机控制方式
	uint16_t water_valve_control_method;//水阀控制方式
	uint16_t supply2_temperature1, supply2_temperature2, supply2_temperature3;//送风温度1 2 3 系统2
	uint16_t supply1_humidity, supply1_humidity, supply1_humidity;//送风湿度1 2 3     系统2
	uint16_t refrigerant_pump1_speed;//制冷剂泵1转速
	uint16_t refrigerant_pump2_speed;//制冷剂泵2转速
	uint16_t ambient_temperature;//环境温度
    tele_c_time update_time;
};

#pragma pack(pop)
#endif
