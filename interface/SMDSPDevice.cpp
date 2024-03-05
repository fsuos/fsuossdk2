#include "SMDSPDevice.hpp"
#include <string.h>
#include <sstream>

SMDSPDevice::SMDSPDevice(): msg_index(0), state(IDLE), addr_(0), isStalled_(false), bNoAutoOpen_(false)
{
	memset(msg_buf, 0, 4096);
	lastTime = boost::posix_time::second_clock::local_time();
}
bool SMDSPDevice::IsProcessing()
{
	return state != IDLE;
}

bool SMDSPDevice::RefreshStatus()
{
	//自动开端口
	if(!bNoAutoOpen_) {
		OpenPort();
	}
	state = RUN;
	msg_index = 0;
	return true;
}

bool SMDSPDevice::Reset()
{
	isStalled_ = false;
	state = IDLE;
	msg_index = 0;
	if(!bNoAutoOpen_)
		return ClosePort();
	return true;
}
bool SMDSPDevice::IsStalled()
{
	//if(isStalled_)
	//	return true;
	if(state != IDLE) {
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = now - lastTime;
		if( diff.total_seconds() > 5) {
			isStalled_ = true;
			return true;
		}
	}
	return false;
}


bool SMDSPDevice::process(unsigned char* data, size_t len)
{
	lastTime = boost::posix_time::microsec_clock::local_time();
	return true;
}

bool SMDSPDevice::Init(std::weak_ptr<SMDController> controller, unsigned int data_id, const Json::Value& settingRoot)
{
	SMDVDevice::Init(controller, data_id, settingRoot);
	if(settingRoot["appSetting"] != Json::nullValue) {
		if(settingRoot["appSetting"].type() == Json::stringValue) {
			if(!settingRoot["appSetting"].asString().empty()) {
				addr_ = atoi(settingRoot["appSetting"].asString().c_str());
			}
		} else if(settingRoot["appSetting"].type() == Json::intValue) {
			addr_ = settingRoot["appSetting"].asInt();
		} else if(settingRoot["appSetting"].type() == Json::objectValue) {
			if(settingRoot["appSetting"]["addr"] != Json::nullValue) {
				addr_ = atoi(settingRoot["appSetting"]["addr"].asString().c_str());
			}
			if(settingRoot["appSetting"]["baud_rate"] != Json::nullValue) {
				baud_rate_ = atoi(settingRoot["appSetting"]["baud_rate"].asString().c_str());
			}
			if(settingRoot["appSetting"]["stop_bit"] != Json::nullValue) {
				stop_bit_size_ = atoi(settingRoot["appSetting"]["stop_bit"].asString().c_str());
			}
			if(settingRoot["appSetting"]["parity"] != Json::nullValue) {
				parity_ = (EnumParity)atoi(settingRoot["appSetting"]["parity"].asString().c_str());
			}

		}
	}
    SMDVDevice::InitSetting(settingRoot);
	return InitSetting(settingRoot);
}

bool SMDSPDevice::OpenPort()
{
	if(controller_.use_count()) {
		if(auto c = controller_.lock())
			return c->OpenPort(data_id_, baud_rate_, char_size_, stop_bit_size_);
	}
#ifdef ENABLE_RCF
	else if(controller_rpc_ != nullptr) {
		return controller_rpc_->OpenPort(data_id_, baud_rate_, char_size_, stop_bit_size_);
	}
#endif
	return false;
}

bool SMDSPDevice::ClosePort()
{
	if(controller_.use_count()) {
		if(auto c = controller_.lock())
			return c->ClosePort(data_id_);
	}
#ifdef ENABLE_RCF
	else if(controller_rpc_ != nullptr) {
		return controller_rpc_->ClosePort(data_id_);
	}
#endif
	return false;
}

bool SMDSPDevice::SendData(const unsigned char *data, size_t len)
{
	if(controller_.use_count()) {
		if(auto c = controller_.lock())
			return c->SendParityData(data_id_, data, len, parity_);
	}
#ifdef ENABLE_RCF
	else if(controller_rpc_ != nullptr) {
		std::string sData((char*)data, len);
		return controller_rpc_->SendParityData(data_id_, sData, parity_);
	}
#endif
	return false;
}

bool SMDSPDevice::SendParityData(const unsigned char *data, size_t len, EnumParity type)
{
	if(controller_.use_count()) {
		if(auto c = controller_.lock())
			return c->SendParityData(data_id_, data, len, type);
	}
#ifdef ENABLE_RCF
	else if(controller_rpc_ != nullptr) {
		std::string cData((const char*)data, len);
		return controller_rpc_->SendParityData(data_id_, cData, (int)type);
	}
#endif
	return false;

}
PLUMA_PROVIDER_SOURCE(SMDSPDevice, 1, 1);
