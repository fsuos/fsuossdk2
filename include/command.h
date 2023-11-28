#ifndef COMMAND_H_
#define COMMAND_H_
#include <string>
#include <stdint.h>

#ifdef ENABLE_RCF
#include <SF/vector.hpp>
#include <SF/string.hpp>
#include <SF/Serializer.hpp>
#endif

class rule_value
{
public:
    bool isCheckBool;
    //CheckThreshold
    std::string key;
    std::string label;//subject，告警的描述部分
    float value;
    //CheckThresholdBool
    int level;
    std::string signal_id;
    std::string signal_name;
    int signal_index;
#ifdef ENABLE_RCF
    void serialize(SF::Archive &ar)
    {
        ar & isCheckBool & key & label & value & level & signal_id & signal_name & signal_index;
    }
#endif
};


#pragma pack(push)
#pragma pack(1)


enum EnumCmdType {
    CMD_LOGIN,
    CMD_OPEN,	//open port
    CMD_DATA,
    CMD_CLOSE,
    PORT_STATUS,
    KEEP_ALIVE, //报警用，比如串口打不开,
    CMD_GETREC_TIMERANG,
    CMD_GETSPEC_RECFILE,
    CMD_REC,
    CMD_CAHPIC,
    FLAG_ADDCAMERA,
    CAMERA_STREAM,
    CMD_EXESHELL,
    POWER_STATUS,
    CMD_STARTDOOR,
    CMD_OPENDOOR,
    CMD_FORCE_OPENDOOR,
    CMD_DISTRIBUTE_CARD,
    CMD_DELETE_CARD,
    CMD_SERVERUPDATE,
    CMD_DELETESERVER,
    CMD_CONNECTREQ,
    CMD_GETACCESS_RECORD,
    CMD_DATA_MARK,
    CMD_DATA_SPACE,
    CMD_DO_DATA,
    CMD_DATA_EVEN,
    CMD_DATA_ODD,
    CMD_DATA_DELAY,
    CMD_LOGIN_FIRST,
    CMD_SP_DATA, //采集好的设备数据,不需要做解析
	CMD_SP_DATA_HISTORY,//采集好的设备历史数据，收到后，直接保存到mongodb
	PORT_STATUS2,//端口命令2,这个上报的时候会传di_len:di_data,ai_len:ai_data
};

typedef struct {
    unsigned char serverIp[16];
    unsigned int sessionNum;
} server_info;

typedef struct { //length 20byte
    unsigned char unknow_1[4];// = {0xff, 0x0, 0x0, 0x0};//此数据通过抓包获取，意义不详
    unsigned int sessionId;
    unsigned char unknow_2[4];// = {0x0, 0x0, 0x0, 0x0};//此数据通过抓包获取，意义不详
    unsigned char respFlag[4];//摄像头回应标志
    unsigned int msgLen;
} camera_msg_header;

typedef struct {
    int portNum;
    int doorId;
    int cardId;
    int diId;
    int doId;
    int userId;
    char userName[8];
} doorInfo;

typedef struct {
//	unsigned char cmd_type;//操作类型   0xF1 - open port, 0xF2 - reply, 0xF3 - send data, 0xF4 - data,
    EnumCmdType cmd_type;
    int num;
    uint32_t len;//数据区数据的长度 从code之后算起，不包括code 单位 Byte
} cmd_header;


namespace REG_TYPE
{
const uint8_t REG_SERVER_REQ = 0;//注册服务器请求
const uint8_t REG_SERVER_RES = 1;//注册服务器回应
const uint8_t REPORT_SERVER_STATUS_REQ = 2;//通知服务器状态请求
const uint8_t REPORT_SERVER_STATUS_RES = 3;//通知服务器状态回应
const uint8_t REG_CLIENT_REQ = 4;//客户端请求服务器地址
const uint8_t REG_CLIENT_RES = 5;//客户端请求服务器地址回应
const uint8_t INVALID_REQ = 6;
}

typedef struct {
    uint8_t cmd;
    uint16_t number;//当cmd==REG_SERVER时，代表priority,比如1，如果有3台服务器各是1,2,3,那么6个客户端i连接将分别得到1,2,3个。
    char server_ip[16];//192.168.111.124
    uint8_t hash[2];//这个用来做服务器认证的，是通过加密算法生成的，不是用于保证数据正确性的
} broker_cmd;


enum operateType {
    CAMERA,
    DOOR
};

enum EnumParity {
    NONE,
    EVEN,
    ODD,
    MARK,
    SPACE,
};
#pragma pack(pop)
struct powerdevice_load {
    float load;//当前负载
    float i;//当前电流
    float i0;//初始电流
};

struct load_flunctuation {
    float load;//当前负载
    float i;//当前电流
    float stable_load;//稳定负载，kalman filter
    float stable_i;//稳定电流

    float sudden_flunctuation;//突变波动率,5%

    //周期波动率
    float i0;//i0
    float i1;//top I1
    float period_flunctuation;//周期波动率

    //电能消耗
    float power_consumption;
};
enum EnumComd {
    RECTIMERANG,
    GETSPECRECFILE,
    CAMREC,
    CAHPIC,
    STARTDOOR,
    OPENDOOR,
    FORCEOPENDOOR,
    DISTRIBUTECARD,
    DELETECARD,
    GETRECORD,
    IDLE
};

#endif //COMMAND_H_
