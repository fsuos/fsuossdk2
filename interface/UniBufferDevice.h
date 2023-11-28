#ifndef UNIBUFFERDEVICE_H
#define UNIBUFFERDEVICE_H

template<typename PClass, int RT_TYPE>
class UniBufferDevice: public PClass 
{
public:
        UniBufferDevice();
        virtual bool InitSetting(const Json::Value &settingRoot);
        bool LoadFromCache(const uint8_t *data, int data_len);
        virtual void RunCheckThreshold(){} 
        void RoundDone() override;
protected:
        int buffer_len_;
        std::unique_ptr<uint8_t[]> buffer_;
};

#endif
