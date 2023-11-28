#ifndef COMMON_INTERFACE_H
#define COMMON_INTERFACE_H

#pragma pack(push)

#pragma pack(1)

struct tele_c_time   //时间
{
	unsigned short year;
	unsigned char  month;
	unsigned char  day;
	unsigned char  hour;
	unsigned char  minute;
	unsigned char  second;
};

#pragma pack(pop)

#endif