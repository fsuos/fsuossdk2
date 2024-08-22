#include "DTSD3366DJ.h"
#include "UniDataDevice.cpp"


DTSD3366DJ::DTSD3366DJ()
{
    device_type_ = "dtsd3366dj";
    baud_rate_ = 9600;
    addr_ = 1;
    //save_interval_ = 600;
}

DTSD3366DJ::~DTSD3366DJ()
{
}


bool DTSD3366DJ::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    state = DTSD3366DJ_R3_0x8000;
    modbus_read_registers(0x8000, 48);
    return true;
}

bool DTSD3366DJ::process_payload(enum tab_type type, size_t len)
{
    switch(state){
      case DTSD3366DJ_R3_0x8000:{
            memcpy(cData.r3_0x8000, tab_reg, sizeof(uint32_t)*48);
            RoundDone();
	    return false;
      }
   }
   return true;
}


float DTSD3366DJ::Get_Value(uint32_t data_id, const std::string& var_name) const
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
    providerVec.push_back(std::make_shared<DTSD3366DJProvider>());
    return std::move(providerVec);
}

#endif
