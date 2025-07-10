#ifndef SMDDEVICEIOCONTROL_HPP
#define SMDDEVICEIOCONTROL_HPP
class SMDDeviceIoControl
{
public:
    virtual int DeviceIoControl(uint32_t data_id, int action, std::string parameter, int retryTime = 3) = 0;
    
    virtual int DeviceIoControl(unsigned int data_id, int ioControlCode, void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) = 0;
    
    virtual bool SendSP(unsigned int data_id, int c_type, const unsigned char* data, size_t len, bool bSave = false) = 0;
    virtual void LockSpPort(uint32_t data_id, bool action) = 0; 
    virtual bool AddEvent(unsigned int data_id, const std::string& signal_name, const std::string& subject) = 0;
    virtual bool SaveDeviceHistory(uint32_t data_id, const std::string& dev_type, const uint8_t* data, size_t data_len) = 0;
    
    virtual int ReadDi(int diNum) = 0;
	virtual float ReadAi(int index) = 0;
	virtual bool SendDoData(int num, char data) = 0;
};

// PLUMA_PROVIDER_HEADER(SMDController);

#endif
