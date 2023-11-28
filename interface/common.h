#ifndef COMMON_H
#define COMMON_H
#include "command.h"

enum SMDDeviceType {
  DEVICE_TYPE_DI=0,
  DEVICE_TYPE_AI=1,
  DEVICE_TYPE_SP=2,
  DEVICE_TYPE_PADDING=3,
  DEVICE_TYPE_SOCKET=4,
  DEVICE_TYPE_POWER=5,
  DEVICE_TYPE_VIRTUAL=6
};


//2023-04-21 这个完全是为了兼容电信的老接口，新的使用新的获取方式
#define MAX_DI_NUMBER 12
#define MAX_AI_NUMBER 8
#define MAX_DO_NUMBER 6
#define MAX_SP_NUMBER 8

/* imem related */
#define READ_IMEM_ADDR 67
#define DEVICE_SYNC 0
#define DEVICE_READY 1
#define DEVICE_FAIL  2

#define DEVICE_UNREG 3
#define DEVICE_REG   4


enum
{
  POWERINFO_CHANNEL_1=0,
  POWERINFO_CHANNEL_2,
  POWERINFO_CHANNEL_3,
  POWERINFO_CHANNEL_4,
  POWERINFO_CHANNEL_MAX
};

enum
{
   ENUM_PA=0,
   ENUM_PB,
   ENUM_PC,
   ENUM_PT,
   ENUM_UARMS,
   ENUM_UBRMS,
   ENUM_UCRMS,
   ENUM_IARMS,
   ENUM_IBRMS,
   ENUM_ICRMS,
   ENUM_ITRMS,
   ENUM_PFA,
   ENUM_PFB,
   ENUM_PFC,
   ENUM_PFT,
   ENUM_FRE,
   ENUM_ESA,
   ENUM_ESB,
   ENUM_ESC,
   ENUM_EST,
   //nanmaoyi:蓝牙新增显示相位夹角等--2017-4-11
   ENUM_RPGA,
   ENUM_RPGB,
   ENUM_RPGC,
   ENUM_RYUAUB,
   ENUM_RYUAUC,
   ENUM_RYUBUC,
   ENUM_UORDER,
   ENUM_IORDER,
   
   ENUM_POWERINFO_NUM
};

#pragma pack(push)
#pragma pack(1)

typedef struct imem_msg_header_
{
	uint8_t soi[6];// = {0xfb, 0x66, 0xfb, 0x66, 0xfb, 0x66};
	uint16_t frame_no;
	uint16_t version;// = 1;
	uint16_t len;
	unsigned int   dev_id;// = 0;
	uint8_t  code;
}imem_msg_header;


typedef struct imem_msg_tailer_
{
	uint16_t csc;
	uint8_t eof[6];// = {0xfe, 0x33, 0xfe, 0x33, 0xfe, 0x33};
}imem_msg_tailer;


typedef struct imem_cmd_data_
{
	uint8_t seq;
	uint16_t offset;
	uint8_t num;
}imem_cmd_data;


typedef struct imem_msg_data_
{
	uint8_t seq;
	uint16_t offset;
	float    data[];
}imem_msg_data;


typedef struct imem_msg_
{
	imem_msg_header header;
	imem_cmd_data  data;
	imem_msg_tailer tailer;
}imem_msg;


/* 301e protocol start */

typedef struct imem_301e_header_
{
	uint8_t s;// = 0x7E;
	uint8_t v;// = 0x11;
	uint8_t cmd;
	uint8_t addr;
	uint8_t com;
	uint8_t frame_property[3];//{0xff, 0xff, 0xff};
	uint16_t len;
	uint8_t csc;
}imem_301e_header;


typedef struct im3190_di_
{
	uint8_t s[6];
	uint8_t seq;
	uint8_t v;
	uint8_t source;
	uint8_t adr;
	uint8_t cid1;
	uint8_t cid2;
	uint16_t len;
	uint16_t data;
	uint16_t csc;
}im3190_di;


typedef struct im3190_ai_
{
	uint8_t s[6];
	uint8_t seq;
	uint8_t v;
	uint8_t source;
	uint8_t adr;
	uint8_t cid1;
	uint8_t cid2;
	uint16_t len;
	//data[10] is temperature data[11] is humidity
	uint16_t data[12];
	uint16_t csc;
}im3190_ai;


/* PMBUs related structures */


typedef struct psma_
{
	uint8_t ver;
	uint8_t addr;
	uint8_t cid1;
	uint8_t cid2;
	uint16_t len;
	uint8_t data[0];
}PMBus_Header;

typedef struct psma_cmd_
{
	uint8_t group;
	uint8_t id;
	uint8_t type;
}psma_cmd;


typedef struct psma_data_channel_
{
	float v_ab;
	//输入相电压A
	short v_a;
	//输入线电压BC
	float v_bc;
	//输入相电压B
	short v_b;
	//输入线电压CA
	float v_ca;
	//输入相电压C
	short v_c;
	//输入频率1
	float f1;
	//输入频率2
	short f2;
	char nc[2];
}psma_data_channel;

struct m810g_ac_40_46
{
	float vu;
 	float vl;
 	float iu;
	float fu;
 	float fl;
 	char  l;
 	float p1;
};

struct zxdu_ac_40_46
{
	float vu;
	float vl;
	float iu;
	float fu;
	float fl;
	char l;
	float p1;
};

struct smu06c_ac_40_46
{
	float vu;
	float vl;
	float iu;
	float fu;
	float fl;
	char l;
	float p1;
};

struct cuc06hd_ac_40_46
{
	float vu;
 	float vl;
 	float iu;
	float fu;
 	float fl;
 	char  l;
 	float p1;
};

typedef struct bma_header_
{
	uint8_t soi[6];
	uint8_t addr;
	uint8_t packno;
	uint8_t cmd;
	uint16_t len;
}bma_header;

typedef struct bma48_data_
{
	unsigned int v[24];
	unsigned int group_v;
	unsigned int group_i;
	unsigned int temperature1;
	unsigned int temperature2;
}bma48_data;

typedef struct bma35_data_
{
	unsigned int v[35];
	unsigned int group_v;
	unsigned int group_i;
	unsigned int temperature[10];
}bma35_data;


#pragma pack(pop)

#include "tele_c_interface.h"

#include "common_func.h"
#endif
