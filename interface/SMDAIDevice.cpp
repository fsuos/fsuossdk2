#include "SMDAIDevice.hpp"

SMDAIDevice::SMDAIDevice():a(1),b(0),value(0.0)
{
	
}

bool SMDAIDevice::Init(std::weak_ptr<SMDController> controller, unsigned  int data_id, const Json::Value& settingRoot)
{
	SMDVDevice::Init(controller, data_id, settingRoot);
	if(settingRoot["appSetting"] != Json::nullValue &&
		settingRoot["appSetting"].type() == Json::objectValue)
	{
		if(settingRoot["appSetting"]["a"] != Json::nullValue)
		{
			a = atof(settingRoot["appSetting"]["a"].asString().c_str());
		}
		if(settingRoot["appSetting"]["b"] != Json::nullValue)
		{
			b = atof(settingRoot["appSetting"]["b"].asString().c_str());
		}
	}
	return InitSetting(settingRoot);
}

PLUMA_PROVIDER_SOURCE(SMDAIDevice, 1, 1);
