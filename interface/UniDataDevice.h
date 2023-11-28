#ifndef UNIDATADEVICE_H
#define UNIDATADEVICE_H

template<typename CDataType, typename PClass, int RT_TYPE>
class UniDataDevice: public PClass 
{
public:
    UniDataDevice();
    virtual bool InitSetting(const Json::Value &settingRoot) override;
    bool LoadFromCache(const uint8_t *data, int data_len) override;
    virtual void RunCheckThreshold(){} 
    void RoundDone() override;
protected:
    CDataType cData;
};

#endif // PMBUSPOWERAC_H
