#ifndef Coolsmart_INTERFACE_H
#define Coolsmart_INTERFACE_H

#include "common_interface.h"

#pragma pack(push)
#pragma pack(1)


struct Coolsmart_Data_t {
    unsigned int data_id;
    uint16_t r3_7[4];
    uint16_t r3_366[44];
	int16_t return_air_temperature;//回风温度
	uint16_t  supply_air_humidity;//回风湿度
	int16_t  supply_air_temperature ;//送风温度
	int16_t  ambient_temperature ;//环境温度
	uint16_t exhaust_pressure;//排气压力
	uint16_t  suction_pressure;//吸气压力
	int16_t exhaust_temperature;//排气温度
	int16_t suction_temperature;//吸气温度
	uint16_t machine_operation status;//机组运行状态
	uint16_t  fan_speed;//风机转速
	uint16_t  compressor_speed;//压机转速
	uint16_t pump_speed;//泵转速
	uint16_t pump_inlet_pressure;//泵进口压力
	uint16_t Pump_outlet_pressure;//泵出口压力
	uint16_t pump_lift;//泵扬程
	uint16_t  condenser_speed;//冷凝器转速
	uint16_t eev_opening;//eev开度
	int16_t inhalation_superheat;//吸气过热量度
	int16_t  discharge_Superheat;//排气过热度
	uint16_t refrigeration;//制冷
	uint16_t heat;//加热
	uint16_t dehumidification;//除湿
	uint16_t humidification;//加湿
	int16_t energy_saving_card_temperature1;//节能卡温度1
	int16_t energy_saving_card_temperature2;//节能卡温度2
	int16_t energy_saving_card_temperature3;//节能卡温度3
	int16_t energy_saving_card_temperature4;//节能卡温度4
	int16_t energy_saving_card_temperature5;//节能卡温度5
	int16_t energy_saving_card_temperature6;//节能卡温度6
	int16_t energy_saving_card_temperature7;//节能卡温度7
	int16_t energy_saving_card_temperature8;//节能卡温度8
	int16_t energy_saving_card_temperature9;//节能卡温度9
	int16_t energy_saving_card_temperature10;//节能卡温度10
	int16_t energy_saving_card_temperature11;//节能卡温度11
	int16_t energy_saving_card_temperature12;//节能卡温度12
	int16_t energy_saving card_temperature13;//节能卡温度13
	int16_t energy_saving_card_temperature14;//节能卡温度14
	int16_t energy_saving_card_temperature15;//节能卡温度15
	int16_t energy_saving_card_temperature16;//节能卡温度16
	uint16_t fan_operating_time;//风机运行时间
	uint16_t humidifier_running_time;//加湿器运行时间
	uint16_t  heating_operation_time1;//加热器1运行时间
	uint16_t compressor_running_time1;//压缩机1运行时间
	uint16_t running_time_of_refrigeration_pump1;//制冷泵1运行时间
	uint16_t supply air_temperature_set_point;//送风温度设定点
	uint16_t return_air_temperature_set_point ;//回风温度设定点
	uint16_t return_air_humidity_set_point;//回风湿度设定点
	uint16_t monitoring_on/off_machine;//监控开关机
	uint16_t monitoring_baud_rate;//监控波特率
	uint16_t monitoring_address;//监控地址

uint16_t alarm1,alarm2,alarm3,alarm4;//告警状态1 2 3 4


_    tele_c_time update_time;
};

#pragma pack(pop)
#endif
