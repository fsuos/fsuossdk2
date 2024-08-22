#include "Sxyw.h"
#include "UniDataDevice.cpp"


Sxyw::Sxyw()
{
    device_type_ = "sxyw";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

Sxyw::~Sxyw()
{
}

bool Sxyw::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
   
    return UniDataDevice<Sxyw_Data_t, SPModbus, RT_Sxyw>::InitSetting(settingRoot);
}

bool Sxyw::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = Sxyw_R3_LEVEL;
    modbus_read_registers(0, 30);
    return true;
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

bool Sxyw::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case Sxyw_R3_LEVEL:{
            for(int i=0; i<15; i++)  {
			copy_to_float(tab_reg + 2*i, (uint8_t*)&cData.r3_level[i]);
		}
		RoundDone();
	    return false;
      }
   }
   return true;
}

float Sxyw::Get_Value(uint32_t data_id, const std::string& var_name) const
{
    if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }
    throw std::out_of_range("不支持变量");
}

#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<SxywProvider>());
    return std::move(providerVec);
}

#endif
