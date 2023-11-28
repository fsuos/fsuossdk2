#ifndef VDEVICEINTERFACE_H
#define VDEVICEINTERFACE_H
#include <stdexcept>
#include <string>


class VDeviceInterface
{
public:
	virtual float Get_Value(uint32_t data_id, const std::string& var_name) const
	{
		throw std::out_of_range("数据未就绪");
	}

	virtual bool LoadFromCache(const std::string& cacheData)
	{
		return LoadFromCache((const uint8_t*)cacheData.data(), cacheData.size());
	}
	virtual bool LoadFromCache(const uint8_t *data, int data_len) = 0;

	virtual uint32_t DataId() const
	{
		return data_id_;
	}
	virtual const std::string& DeviceType() const
	{
		return device_type_;
	}
	//virtual const std::string& DeviceCategory() const { return device_category_; }
	
	
	//ioctl没有ms的DeviceIoControl直观,返回<0是找不到设备
	//仿照https://msdn.microsoft.com/en-us/library/ff542856%28v=vs.85%29.aspx
	virtual int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
	{
		/*not implemented */return -2;
	}

	virtual void RoundDone() {} ;

	virtual bool UseControllerDb() final
	{
		return bUseControllerDb;
	}
public:
	unsigned int data_id_ = 0;
	//2017-5-21 张杨，记不起来当时为啥加这个device_category_
	//std::string device_category_;
	std::string device_type_;

	bool bUseControllerDb = false;
};

#endif // VDEVICEINTERFACE_H
