#ifndef UNIDATADEVICE_H
#define UNIDATADEVICE_H

template<typename CDataType, typename PClass, int RT_TYPE>
class UniDataDevice: public PClass 
{
public:
    UniDataDevice();
    
    virtual ~UniDataDevice();
    
    virtual bool InitSetting(const Json::Value &settingRoot) override;
    bool LoadFromCache(const uint8_t *data, int data_len) override;
    virtual void RunCheckThreshold(){}
    void RoundDone() override;
    
    void DoCheck(std::vector<rule_value> rvVec);
protected:
    //这个地方是如果cData没有4对齐，访问cData中的成员变量可能会出问题，cData内部在声明的时候，会注意内存的布局
    CDataType __attribute__((__aligned__(4))) cData;
    
    int b_mode_ = 1;//1:联通 2:电信

    boost::posix_time::ptime lastJsonTime_;
    std::mutex jsonValueMutex;
    Json::Value jsonValue;
};

#endif // PMBUSPOWERAC_H
