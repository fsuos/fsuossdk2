#ifndef KSTAR_H
#define KSTAR_H

#include "SMDSPDevice.hpp"
#include "kstar_interface.h"

enum EA86_Status{EA86_IDLE,EA86_G1,EA86_G2,EA86_G3,EA86_I,EA86_GF,EA86_QI,EA86_F};

class EA86:public SMDSPDevice
{
	EA86_Status mCmdStatus;
	EA86_t cData;
    unsigned int dataNeed;

	int buffer_len_;
	std::unique_ptr<uint8_t[]> buffer_;
 	
public:
	EA86();
	~EA86();	
	float Get_Value(uint32_t data_id, const std::string &var_name) const;
	bool InitSetting(const Json::Value& ) override;
	void RoundDone();
	bool process(unsigned char *data, size_t len) override;
    bool process_payload(unsigned char * data, size_t len);
	bool RefreshStatus() override;
};

PLUMA_INHERIT_PROVIDER(EA86, SMDSPDevice)
#endif
