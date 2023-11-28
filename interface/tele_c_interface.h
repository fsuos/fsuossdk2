#ifndef TELE_C_INTERFACE_
#define TELE_C_INTERFACE_

#define TELE_C_SOI 0x7E7C6B5A

#define LOGIN 101
#define LOGIN_BACK 102
#define LOGOUT 103
#define LOGOUT_BACK 104

#define SET_DYN_ACCELSC_MODE 401
#define DYN_ACCELSC_MODE_ACK 402

#define SET_POINT 1001
#define SET_POINT_ACK 1002

#define HEART_BEAT 1201
#define HEART_BEAT_ACK 1202

#define RT_HUMID 4997
#define RT_TEMPERATURE 4998
#define RT_CAMERA  4999
//Our msg start from 5000

#define NAME_LENGTH 40
#define USER_LENGTH 20
#define PASSWORD_LEN 20
#define EVENT_LENGTH 160
#define ALARM_LENGTH 165
#define LOGIN_LENGTH 100
#define DES_LENGTH 40
#define UNIT_LENGTH 8

#include <stdint.h>
#include <vector>
enum EnumRightMode {
    INVALID =0,
    LEVEL1 = 1,
    LEVEL2 = 2
};

enum EnumResult {
    FAILURE = 0,
    SUCCESS = 1
};

enum EnumEnable {DISABLE=0,ENABLE=1};

enum EnumAcceSCMode {ASK_ANSWER=0,CHANGE_TRIGGER=1,TIME_TRIGGER=2,STOP=3};

enum EnumType {STATION=0,DEVICE=1,DI=2,AI=3,DO=4,AO=5};

enum EnumState {NOALARM=0,CRITICAL=1,MAJOR=2,MINOR=3,HINT=4,OPEVENT=5,INVALID1=6};


#define GET_AC_CHANNEL_FIELD(RET,FIELD) RET get_ac_channel_##FIELD(int index){\
        if(index >= 0 && index < channel_count)\
        {\
            return channels[index].FIELD;\
        }\
        return 0;\
    }

#include "common_interface.h"
#pragma pack(push)

#pragma pack(1)


typedef struct tele_c_ta_ { //模拟量的值
    EnumType Type;//数据类型
    unsigned int     ID;//数据标识ID
    unsigned int      LSCID;//LSC ID号
    float    Value;
    EnumState Status;//状态
} tele_c_ta;

typedef struct tele_c_td_ { //数字量的值
    EnumType Type;//数据类型
    unsigned int     ID;//数据标识ID
    unsigned int      LSCID;//LSC ID号
    char    Value;
    EnumState Status;//状态
} tele_c_td;

typedef struct tele_c_header_ { //
    int soi;//must be 0x7E7C6B5A
    uint32_t len;
    unsigned int serial_no;
    int pk_type;//报文类型
} tele_c_header;

typedef struct tele_c_info_login_ {
    char username[USER_LENGTH];
    char password[PASSWORD_LEN];
} tele_c_info_login;

//用户请求实时数据报文
typedef struct tele_c_info_dyn_access_ {
    unsigned int TerminalID;   //PSC监控终端ID
    unsigned int GroupID;   //响应模式数据包的序号
    EnumAcceSCMode mode;    //数据发送方式
    unsigned int PollingTime;   //定时方式时的发送间隔秒数，小于1无效，若出现小于1的值，则按等于1处理。
    unsigned int Cnt;   //请求的实时数据的数量
    unsigned int Ids[0];    //相应的数据ID号，
    //当ID号为某一设备的ID号时，则要求返回该设备包含的所有监控点的值（此时该设备ID号的DDD字段为全0），
    //当ID为某LSC或局站的ID时，返回的实时数据量为0，方式设定为失败，如果返回－1则表示节点数过多，报文过长，如果返回是－2，则表示无相应ID号
} tele_c_info_dyn_access;

//用户请求实时数据应答报文
typedef struct tele_c_info_dyn_access_ack_ {
    unsigned int TerminalID;    //PSC监控终端ID
    unsigned int GroupID;   //响应模式数据包序号，与客户包相等
    EnumResult Result;  //方式设定成功与否的标志，Cnt的值小于等于0时，Result为0,反之为1
    unsigned int Cnt;   //返回数据值的数量，如果返回－1则表示节点数过多，报文过长，如果返回是－2，则表示无相应ID号
    unsigned int Ids[0];
} tele_c_info_dyn_access_ack;

typedef struct tele_c_xjlp_ {
    unsigned int data_id;
    unsigned int doorId;//accessId
    bool  isDoorOpen;//open:0(false); close :1(true)
    tele_c_time update_time;
} tele_c_xjlp;

struct tele_c_imem12 {
    unsigned int          data_id;
    float         channel1[67];
    float         channel2[67];
    float         channel3[67];
    float         channel4[67];
    tele_c_time update_time;

    float get_float(int index, int offset)
    {
        if(offset >=0 && offset < 65) {
            switch(index) {
            case 0:
                return channel1[offset];
            case 1:
                return channel2[offset];
            case 2:
                return channel3[offset];
            case 3:
                return channel4[offset];
                break;
            default:
                return 0;
            }
        }
        return 0;
    }
};

typedef struct tele_c_liebert_ {
    unsigned int data_id;
    int temperature_set;
    int temperature_tolerate;
    int humidity_set;
    int humidity_tolerate;
    char cooling_glycool;
    int temperature_reading;
    int humidity_reading;
    int heat_cool_percentage;
    int heat_cool_level;
    char degrees_fc;
    tele_c_time update_time;
} tele_c_liebert;


/********* 60_44 **************
1	回风高温	1
2	回风低温	1
3	回风高湿	1
4	回风低湿	1
5	地板溢水	1
6	系统消防告警	1
7	防火阀告警	1
8	电网频率异常	1
9	电源过压告警	1
10	电源欠压告警	1
11	电源缺相告警	1
12	电源逆相告警	1
13	湿膜加湿高水位	1
14	湿膜加湿低水位	1
15	冲洗阀无法开启	1
16	冲洗阀关闭不严	1
17	新风系统滤网脏堵	1
18	室内温感1失效	1
19	室内温感2失效	1
20	室内温感3失效	1
21	室内温感4失效	1
22	室内温感5失效	1
23	室内湿感1失效	1
24	室内湿感2失效	1
25	室内湿感3失效	1
26	室内湿感4失效	1
27	室内湿感5失效	1
28	出风风机故障	1
29	保留	1
30	出风风机变频通信故障	1
31	保留	1
32	保留	1
33	保留	1
34	保留	1
35	保留	1
其他	保留（总共135）	1

告警字节描述：   00H：正常    20H：无此报警类型           01H：故障
********************************************************/

typedef struct tele_c_freshair_ {
    unsigned int data_id;

    //60_42 获取模拟量量化后数据（定点数）
    float temperature1;				//1	室内温度1	2
    float temperature2;				//2	室内温度2	2
    float temperature3;				//3	室内温度3	2
    float temperature4; 			//4	室内温度4	2
    float temperature5;				//5	室内温度5	2
    float humidity1;				//6	室内湿度1	2
    float humidity2;				//7	室内湿度2	2
    float humidity3;				//8	室内湿度3	2
    float humidity4;				//9	室内湿度4	2
    float humidity5;				//10	室内湿度5	2
    float wind_temperature;			//11	出风温度	2
    float wind_humidity;			//12	出风湿度	2
    float outside_temperature;		//13	室外温度	2
    float outside_humidity;			//14	室外湿度	2
    float humidifier_current;		//15	加湿器电流 2
    float average_temperature;		//16	室内平均温度	2
    float average_humidity;			//17	室内平均湿度	2
    unsigned short reserve_60_42_1;	//18	保留	2
    unsigned short reserve_60_42_2;	//19	保留	2
    float highest_temperature;		//20	最高室内温度	2




    //60_43 获取设备状态及系统运行状态,状态字节含义:00H:停止(关机)	   01H：运行(开机)
    char runstate_alert;	//1	公共告警	1
    char runstate_fan;		//2	内风机	1
    char runstate_r1;		//3	保留	1
    char runstate_r2;		//4	保留	1
    char runstate_r3;		//5	保留	1
    char runstate_r4;		//6	保留	1
    char runstate_drain;	//7	湿帘加湿排水	1
    char runstate_fill;		//8	湿帘加湿注水	1
    char runstate_pump;		//9	湿帘加湿水泵	1
    char runstate_ac;		//10	外部空调	1

    //60_44 获取告警状态
    char alert[135];//00H：正常    20H：无此报警类型  ,01H：故障

    //60_47 参数设置
    float setting_temperature;		//1	温度设定点 2
    float setting_humidity;			//2	湿度设定点 2
    float high_temperature_alert;	//3	高温告警点 2
    float low_temperature_alert;	//4	低温告警点 2
    float high_humidity_alert;		//5	高湿报警点 2
    float low_humidity_alert;		//6	低湿报警点 2

    tele_c_time update_time;
    float get_float(int index)
    {
        if(index == 0) {
            return temperature1;
        } else if(index <17) {
            return *(&temperature1+index);
        } else if(index == 17) {
            return highest_temperature;
        } else if(index == 18) {
            return setting_temperature;
        } else {
            return *(&setting_temperature+index-18);
        }
    }
    char get_c(int index)
    {
        if(index == 0) {
            return runstate_alert;
        } else if(index < 144) {
            return *(&runstate_alert + index);
        }
        return 0;
    }
} tele_c_freshair;

typedef struct tele_c_liebert_ups_ {
    unsigned int data_id;

    unsigned short a[32];//重要遥测-A帧
    unsigned int d1[3];//遥信D1帧,F0-F2三个功能吗
    tele_c_time update_time;
    unsigned short get_a(int index)
    {
        if(index<32) {
            return a[index];
        } else {
            return 0;
        }
    }
    unsigned int get_d1(int index)
    {
        if(index<3) {
            return d1[index];
        } else {
            return 0;
        }
    }
} tele_c_liebert_ups;
//Real Data Structure End

struct tele_c_aeg_ms10se {
    unsigned int data_id;
    //系统设置参数区
    //unsigned int reg1[5];
    unsigned int pt1; //PT1
    unsigned int pt2; //PT2
    unsigned int ct1; //CT1
    unsigned int ct2; //CT2

    //基本测量参数地址区,0x130 - 0x153
    //unsigned short reg2[35];
    unsigned int f; //频率
    unsigned int v1; //相电压V1
    unsigned int v2; //相电压v2
    unsigned int v3; //相电压v3
    unsigned int vvavg; //相电压均值
    unsigned int v12; //线电压v12
    unsigned int v23; //线电压v23
    unsigned int v31; //线电压v31
    unsigned int vlavg; //线电压均值
    unsigned int i1; //相电流i1
    unsigned int i2; //相电流i2
    unsigned int i3; //相电流i3
    unsigned int iavg; //三相电流均值
    unsigned int in; //中线电流
    int p1; //分相有功功率p1
    int p2; //分相有功功率p2
    int p3; //分相有功功率p3
    int psum; //系统有功功率
    int q1; //分相无功功率q1
    int q2; //分相无功功率q2
    int q3; //分相无功功率q3
    int qsum; //系统无功功率
    unsigned int s1; //分相视在功率s1
    unsigned int s2; //分相视在功率s2
    unsigned int s3; //分相视在功率s3
    unsigned int ssum; //系统视在功率
    int pf1; //分相功率因数pf1
    int pf2; //分相功率因数pf2
    int pf3; //分相功率因数pf3
    int pf; //系统功率因数
    long psum2; //系统有功功率 二次侧/10
    long qsum2; //系统无功功率 二次侧/10
    long ssum2; //系统视在功率 二次侧/10

    //电度参量地址区,0x156-0x165
    //unsigned short reg3[15];
    float ep_imp; //有功输入电度
    float ep_exp; //有功输出电度
    float eq_imp; //感性无功电度
    float eq_exp; //容性无功电度
    float ep_total; //总有功电度
    float ep_net; //净有功电度
    float eq_total; //总电度
    float eq_net; //净无功电度

    //DI 数据地址区,02,0-5
    unsigned char  di[6];

    //DO 数据地址区,01,0-5
    unsigned char d_o[6];

    tele_c_time update_time;
};


struct tele_c_amf25 {
    unsigned int data_id;
    uint16_t l1_n;//发电机l1-n相电压
    uint16_t l2_n;
    uint16_t l3_n;
    uint16_t l1_l2;//发电机l1-l2线电压
    uint16_t l2_l3;
    uint16_t l3_l1;
    uint16_t l1_i;//l1负载电流
    uint16_t l2_i;
    uint16_t l3_i;
    uint16_t rpm;//转速，1500/分钟
    float freq;//Hz:501, 保存时候需要除以10
    uint16_t p;//有功功率
    uint16_t l1_p;//L1有功功率
    uint16_t l2_p;
    uint16_t l3_p;
    uint16_t kva;//标称视在功率
    uint16_t load_kvar;//无功功率
    uint16_t load_kvar_l1;//l1无功功率
    uint16_t load_kvar_l2;//l2无功功率
    uint16_t load_kvar_l3;//l3无功功率
    float     pf;//总功率因数，要除2
    float     l1_pf;//
    float     l2_pf;
    float     l3_pf;
    uint16_t load_kva;//总视在功率
    uint16_t load_kva_l1;//l1视在功率
    uint16_t load_kva_l2;//l2视在功率
    uint16_t load_kva_l3;//l3视在功率
    uint16_t mains_l1_n;//干线l1-n相电压
    uint16_t mains_l2_n;//干线l2-n相电压
    uint16_t mains_l3_n;//干线l3-n相电压
    uint16_t mains_l1_l2;//干线l1-l2线电压
    uint16_t mains_l2_l3;//干线l2-l3线电压
    uint16_t mains_l3_l1;//干线l3-l1线电压
    uint16_t mains_freq;//干线频率
    float    earth_fault;//接地故障,除100
    float    battery_voltage;//电池电压,除10
    float    dplus;//充电电压，除10
    float    oil_presure;//油压，除10
    uint16_t engine_temp;//引擎温度
    uint16_t bin_inputs;/* 0  GCB Feedback
  1  MCB Feedback
  2  Emergency Stop
  3  低油压
  4  高水温
  5  Remote TEST
  6  Rem Start/Stop*/
    uint16_t bin_outputs;/*  0  Starter
  1  Fuel Solenoid
  2  GCB Close/Open
  3  MCB Close/Open
  4  Prestart
  5  Horn
  6  Running
   */
    uint16_t iom_bin_inp;/* 0  IOM BI1 Alarm
  1  IOM BI2 Alarm
  2  IOM BI3 Alarm
  3  IOM BI4 Alarm
  4  IOM BI5 Alarm
  5  IOM BI6 Alarm
  6  IOM BI7 Alarm
  7  IOM BI8 Alarm */
    uint16_t engine_state;
    uint16_t breaker_state;
    /* 20  Init
        21  Not ready
        22  Prestart
        23  Cranking
        24  Pause
        25  Starting
        26  Running
        27  Loaded
        28  Stop
        29  Shutdown
        30  Ready
        31  Cooling
        32  EmergMan
        33  MainsOper
        34  MainsFlt
        35  MainsFlt
        36  IslOper
        37  MainsRet
        38  Brks Off
        39  No Timer
        40  MCB Close
        41  ReturnDel
        42  Trans Del
        43  Idle Run
        44  MinStabTO
        45  MaxStabTO
        46  AfterCool
        47  GCB Open
        48  StopValve
        49  Start Del
        50  (1Ph)
        51  (3PD)
        52  (3PY)
        53  MRS Mode      */
    float run_hours;//除10
    uint16_t maintainance;//
    uint16_t num_starts;//
    uint32_t genset_kwh;//总发电有功电量
    uint32_t genset_kvarh;//总发电无功电量
    uint32_t num_estops;//紧急停车数
    uint32_t shutdowns;//关机数
    tele_c_time update_time;
};

struct tele_c_k200 {
    unsigned int data_id;
    uint16_t set_switch; //设置开关机
    int16_t set_temp; //设定温度
    uint16_t set_humid; //设定湿度
    uint16_t switchs;  //开关机状态
    uint16_t fan;  //风机状态
    uint16_t heating; //制热
    uint16_t cooling; //制冷
    uint16_t humidify; //加湿
    uint16_t dehumidfy; //除湿
    int16_t air_temp; //回风温度
    uint16_t air_humid; //回风湿度
    uint16_t wind_temp_alert; //送风温度告警
    uint16_t out_temp_probe_alert; //室外温度探头告警
    uint16_t air_humid_probe_alert; //回风湿度探头告警
    uint16_t wind_temp_probe_alert; //送风温度探头告警
    uint16_t air_temp_probe_alert; //回风温度探头告警
    uint16_t low_humid_alert; //低湿告警
    uint16_t high_humid_alert; //高湿告警
    uint16_t low_temp_alert; //低温告警
    uint16_t high_temp_alert; //高温告警
    uint16_t airstrainer; //空气过滤网
    uint16_t heater_overload; //加热器过载
    uint16_t fan_overload; //风机过载
    uint16_t air_flow_loss; //气流丢失
    uint16_t low_pressure_compressor; //压缩机低压
    uint16_t high_pressure_compressor; //压缩机高压
    uint16_t water_switch_alert; //水流开关告警
    uint16_t low_pressure_compressor2; //压缩机2低压
    uint16_t high_pressure_compressor2; //压缩机2高压
    uint16_t smoke_alert; //烟雾告警
    uint16_t user_alert; //用户告警
    uint16_t overflow_alert; //溢流告警
    uint16_t no_humid_current; //无加湿电流
    uint16_t humidifier_water_shortage; //加湿器缺水
    uint16_t high_hunidifier_current; //加湿器电流过大
    tele_c_time update_time;

};

struct tele_c_pmac600b {
    unsigned int data_id;
    float ua; //A相电压
    float ub; //B相电压
    float uc; //C相电压
    float ia;//A相电流
    float ib;//B相电流
    float ic;//C相电流
    float psum; //有功总和
    float qsum; //无功总和
    float pf_sum; //功率因数总和
    float pa;//A相有功功率
    float pb;//B相有功功率
    float pc;//C相有功功率
    float qa;//A相无功功率
    float qb;//B相无功功率
    float qc;//C相无功功率
    float cosfi_a;//A相功率因数
    float cosfi_b;//B相功率因数
    float cosfi_c;//C相功率因数
    float freq; //频率
    float ep_sum; //有功电度总和
    float eq_sum; // 无功电度总和
    float ep_imp; //输入有功电度
    float ep_exp; //输出有功电度
    float eq_imp; //输入无功电度
    float eq_exp; //输出无功电度
    float switchs; //开关量状态
    float relays; //继电器状态
    float i0; //零序电流
// 以下数据无法读出
    short addr; 	//40201 通讯地址
    short ct;  		//40202 CT变化
    short tmode; 	//40203 接线模式,0:三相四线,1:三项三线
    short pt; 		//40204 PT变化
    short rate; 	//40205 波特率 0:4800,1:9600
//  float channel1; 模拟通道1对象
//  float channel2; 模拟通道1对象
//  float relay1_oper_mode; 继电器1动作模式
//  float relay1_oper_obj; 继电器1动作对象
//  float relay1_oper_high; 继电器1动作上限
//  float relay1_oper_low; 继电器1动作下限
//  float relay1_oper_time; 继电器1动作时间
//  float relay2_oper_mode; 继电器2动作模式
//  float relay2_oper_obj; 继电器2动作对象
//  float relay2_oper_high; 继电器2动作上限
//  float relay2_oper_low; 继电器2动作下限
//  float relay2_oper_time; 继电器2动作时间
//  float elec_clear; 电度清零
//  float software; 软件版本号
//  float hardware; 硬件版本号
//  float product; 产品序列号
    tele_c_time update_time;

};

struct tele_c_pmac600a {
    unsigned int data_id;
    float cur; //电流
    float vol; //电压
    float ap; //有功功率
    float rp; //无功功率
    float pf; //功率因数
    float freq; //频率
    float sc; //开关量通道
    float rc; //继电器通道
    float ep; //有功电度
    float eq; //无功电度
//  数据无法读出
//  unsigned int addr; //通讯地址
//  unsigned int ct; //CT变比
//  unsigned int cbr; //通讯波特率
//  unsigned int ao; //模拟量对象
//  unsigned int rom; //继电器动作模式
//  unsigned int roj; //继电器动作对象
//  unsigned int roh; //继电器动作上限
//  unsigned int rol; //继电器动作下限
//  unsigned int rot; //继电器动作时间
//  unsigned int ec; //电度清零

    tele_c_time update_time;
};


struct tele_c_data_mate3000 {
    unsigned int data_id;
    //获取模拟量参数 --60_42
    float room_temp; //室内温度
    float room_humid; //室内湿度
    float outdoor_temp; //室外温度
    //获取开关机状态 --60_43
    unsigned char air_state; //空调状态
    //获取系统参数 --60_47
    float temperature; //开机温度
    float humidity; //关机湿度
    float set_temp; //温度设定点
    float temp_pric; //温度偏差
    float set_humid; //湿度设定点
    float humid_pric; //湿度偏差
//  //获取系统监测模块时间 --60_4D
//  unsigned short sys_year;
//  unsigned char sys_month;
//  unsigned char sys_day;
//  unsigned char sys_hour;
//  unsigned char sys_minute;
//  unsigned char sys_seconds;
    //获取机组状态 --60_82
    unsigned char switch_status; //开关机状态
    unsigned char fan_status; //风机状态
    unsigned char cool_status; //制冷状态
    unsigned char heat_status; //加热状态
    unsigned char humid_status; //加湿状态
    unsigned char dehumid_status; //除湿状态
    unsigned char alert_status; //告警状态
    unsigned char high_press_alarm; //高压报警
    unsigned char low_press_alarm; //低压报警
    unsigned char high_temp_alarm; //高温报警
    unsigned char low_temp_alarm; //低温报警
    unsigned char high_humid_alarm; //高湿报警
    unsigned char low_humid_alarm; //低温报警
    unsigned char power_failer_alarm; //电源故障告警
    unsigned char short_cycle_alarm;  //短周期告警
    unsigned char custom_alarm1; //用户自定义告警1
    unsigned char custom_alarm2; //用户自定义告警2
    unsigned char main_fan_mainten_alarm; //主风机维护告警
    unsigned char humid_mainten_alarm; //加湿器维护报警
    unsigned char filter_mainten_alarm; //过滤网维护报警
    unsigned char com_failer_alarm; //通讯故障报警
    unsigned char coil_freeze_alarm; //盘管冻结告警
    unsigned char humid_fault_alarm; //加湿器故障报警
    unsigned char sensor_miss_alarm; //传感器板丢失告警
    unsigned char gas_temp_fault_alarm; //排气温度故障告警
    unsigned char power_miss_fault_alarm; //电源丢失故障告警
    unsigned char power_undervol_alarm; //电源过欠压报警
    unsigned char power_phase_alarm; //电源缺相报警
    unsigned char power_freq_alarm; //电源频率偏移报警
    unsigned char floor_overflow_alarm; //地板溢水报警
    unsigned char save_card_failure; //节能卡故障

    //获取当前机组状态  --60_83
    //unsigned char unit_status; //机组状态
    //unsigned char unit_prop; //机组属性
    //unsigned char high_press_lock; //高压锁定
    //unsigned char low_press_lock; //低压锁定
    //unsigned char exhaust_lock; //排气锁定

    tele_c_time update_time;
};

struct tele_c_data_mec10 {
    unsigned int data_id;
    char mode;//0xE0 备份
    char on_status;//0x04 自动
    short speed;
    char front_status;
    char alert1;
    char alert2;
    char alert3;
    char alert4;
    short battery;
    short ai[7];
    short freq;
    int   run_time;//运行时间
    short up_time;//启动次数
    short count;//计时器
    char year;
    char month;
    char day;
    char hour;
    char minute;
    char second;
    tele_c_time update_time;
};


struct tele_c_detal_ups {
    unsigned int data_id;

    //　7e 30 30 50 30 30 33 52 41 54  RAT
    float irv; //输入额定电压
    float irf; //输入额定频率
    float orv; //输出额定电压
    float orf; //输出额定频率
    float rca; //额定容量
    float orp; //输出额定功率
    float dcrt; //延时关闭UPS的额定时间
    float obv; //额定电池电压

    //　7e 30 30 50 30 30 33 53 54 42  STB
    unsigned char bs; //电池状态 -- 0：很好，1：虚弱，2：需要更换
    unsigned char bvs; //电池电压状态 -- 0：满，1：低，2：电压耗尽
    unsigned char bcs; //电池的充电状态 -- 0：浮充，1：均充，2：停止充电，3：放电
    float bt; //电池时间
    float bv; //电池电压
    float bc; //电池电流
    float bte; //电池温度
    float bca; //电池容量--百分比

    //　7e 30 30 50 30 30 33 53 54 49  STI
    float iph; //输出相数
    float ifa; //A相输入频率
    float iva; //A相输入电压
    float iia; //A相输入电流
    float ifb; //B相输入频率
    float ivb; //B相输入电压
    float iib; //B相输入电流
    float ifc; //C相输入频率
    float ivc; //C相输入电压
    float iic; //B相输入电流

    //　7e 30 30 50 30 30 33 53 54 4f STO
    float vos; //电压输出源　　0：表示正常，是市电输入给UPS，UPS输出电压。1：表示电池供应。2：旁路供应。3：降压供应。4：升压推挽供应。5：手动旁路供应.6：其他方式7：没有。
    float of; //输出频率
    float oph; //输出相数
    float ova; //A相输出电压
    float oia; //A相输出电流
    float la; //A相负载百分比
    float ovb; //B相输出电压
    float oib; //B相输出电流
    float lb; //B相负载百分比
    float ovc; //C相输出电压
    float oic; //C相输出电流
    float lc; //C相负载百分比

    //　7e 30 30 50 30 30 33 53 54 50 STP
    float bof; //旁路输出频率
    float bop; //旁路输出相数
    float bova; //A相旁路输出电压
    float bovb; //B相旁路输出电压
    float bovc; //C相旁路输出电压

    //　7e 30 30 50 30 30 33 53 54 41 STA
    unsigned char it_alert; //UPS机内温度 -- 0：正常 1：温度超高
    unsigned char iv_alert; //UPS输入电压低或输入电压不稳定 -- 0：正常 1：输入电压低或输入电压不稳定。
    unsigned char ov_alert; //UPS输出电压低或输出电压不稳定。--0：正常 1：输出电压低或输出电压不稳定。
    unsigned char ol_alert; //UPS超载
    unsigned char bov_alert; //UPSUPS旁路电压低或旁路电压不稳定
    unsigned char ovc_alert; //UPS输出电压关闭   0--打开  1--关闭
    unsigned char rm_alert; //UPS远程关闭  --　为0时：UPS输出正常, 为1时：UPS输出远程关闭。
    unsigned char bcf_alert; //UPS的电池充电故障 -- 0：正常 1：故障
    unsigned char fa_alert; //UPS风扇故障 -- 0：正常
    unsigned char mcf_alert; //UPS机器普通故障 -- 0：正常
    unsigned char oph_alert; //UPS输出功率滞后 0:正常 1：滞后
    unsigned char bu_alert; //UPS蜂鸣器报警 0:正常
    unsigned char wem_alert; //UPS工作在经济模式   0：在 1：不在
    unsigned char iov_alert; //逆变器输出电压不稳定 0：正常
    unsigned char unknown1; //未知告警1
    unsigned char unknown2; //未知告警1
    unsigned char unknown3; //未知告警1

    //7e 30 30 50 30 30 33 54 58 56 TXV
    float hov; //UPS最低工作电压
    float lov; //UPS最高工作电压

    tele_c_time update_time;
};

struct tele_c_met05 {
    //8个字节的返回码
    unsigned int data_id;
    unsigned char urgent_alarm1;//NO1 1-8紧急告警
    unsigned char urgent_alarm2;
    unsigned char urgent_alarm3;
    unsigned char urgent_alarm4;
    unsigned char urgent_alarm5;
    unsigned char urgent_alarm6;
    unsigned char urgent_alarm7;
    unsigned char urgent_alarm8;
    unsigned char non_urgent1;//NO2 1-4非紧急
    unsigned char non_urgent2;
    unsigned char non_urgent3;
    unsigned char non_urgent4;
    unsigned char urgent9;//NO2 9-12紧急
    unsigned char urgent10;
    unsigned char urgent11;
    unsigned char urgent12;
    unsigned char non_emergency5;//No3 5-12非紧急
    unsigned char non_emergency6;
    unsigned char non_emergency7;
    unsigned char non_emergency8;
    unsigned char non_emergency9;
    unsigned char non_emergency10;
    unsigned char non_emergency11;
    unsigned char non_emergency12;
    unsigned char air_switch1;//No4 1-8空气开关
    unsigned char air_switch2;
    unsigned char air_switch3;
    unsigned char air_switch4;
    unsigned char air_switch5;
    unsigned char air_switch6;
    unsigned char air_switch7;
    unsigned char air_switch8;
    unsigned char air_switch9;//No4 9-16空气开关
    unsigned char air_switch10;
    unsigned char air_switch11;
    unsigned char air_switch12;
    unsigned char air_switch13;
    unsigned char air_switch14;
    unsigned char air_switch15;
    unsigned char air_switch16;
    unsigned char air_switch17;//No5 17-24空气开关
    unsigned char air_switch18;
    unsigned char air_switch19;
    unsigned char air_switch20;
    unsigned char air_switch21;
    unsigned char air_switch22;
    unsigned char air_switch23;
    unsigned char air_switch24;
    unsigned char air_switch25;//No6 25-32空气开关
    unsigned char air_switch26;
    unsigned char air_switch27;
    unsigned char air_switch28;
    unsigned char air_switch29;
    unsigned char air_switch30;
    unsigned char air_switch31;
    unsigned char air_switch32;
    unsigned char fuse_Bit2;//NO8 Bit2-熔丝
    unsigned char overvoltage_Bit1;//Bit1-过压
    unsigned char Undervoltage_Bit0;//Bit0-欠压
    //余为 0
    tele_c_time update_time;
};
/// door xjl header
struct tele_c_door {
    unsigned int data_id;
    char door1;
    char door2;
    tele_c_time update_time;
};
#pragma pack(pop)

/*#include "dk04_interface.h"
#include "dk04c_interface.h"
#include "psma_interface.h"
#include "m810g_interface.h"
#include "psm15a_interface.h"
#include "smu06c_interface.h"
#include "zxdu_interface.h"*/

#endif
