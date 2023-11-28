#ifndef SMDCONTROLLER_HPP
#define SMDCONTROLLER_HPP
#include "command.h"
#include <iostream>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/mutex.hpp>
#include <functional>
#include <map>
#include <stdexcept>

#ifndef SMDDEVICE
#include <bsoncxx/builder/basic/document.hpp>
#include <tbb/concurrent_unordered_map.h>
using tbb::concurrent_unordered_map;
#include "spdlog/spdlog.h"
#endif

#ifdef ENABLE_RCF


#include <RCF/Idl.hpp>

RCF_BEGIN(SMDControllerRPC, "SMDControllerRPC")
RCF_METHOD_R2(bool, DoBatchCheckThreshold, unsigned int, const std::vector<rule_value>&);
RCF_METHOD_R4(bool, DoCheckThreshold, unsigned int, const std::string&, float, int);
RCF_METHOD_R4(bool, SendSP, unsigned int, int, const std::string&, bool);
RCF_METHOD_R4(bool, OpenPort, unsigned int, int, int, int);
RCF_METHOD_R1(bool, ClosePort, unsigned int);
RCF_METHOD_R2(bool, SendData, unsigned int, const std::string&);
RCF_METHOD_R2(bool, SendDelayData, unsigned int, int);
RCF_METHOD_R3(bool, SendParityData, unsigned int, const std::string&, int);
RCF_METHOD_R2(bool, SendDoData, int, char);
RCF_METHOD_R1(int, ReadDi, int);
RCF_METHOD_R1(float, ReadAi, int);
RCF_METHOD_R2(float, Get_SP_Value, unsigned int, const std::string&);
RCF_METHOD_R3(bool, SaveCache, const std::string&, const std::string&, unsigned int);
RCF_METHOD_R2(bool, GetCacheValue, const std::string&, const std::string&);
RCF_METHOD_R2(bool, GetCacheInt, const std::string&, int&);
RCF_METHOD_R3(int, StartSocket, uint32_t, const std::string&, uint16_t);
RCF_METHOD_R2(int, CloseSocket, uint32_t, int);
RCF_METHOD_R3(int, SendSocketData, uint32_t, int, const std::string&);
RCF_END(SMDController);
#endif

using boost::asio::ip::tcp;
class SMDSocketDevice;
class SMDController
{
public:
	enum { LEVEL1 = 1, LEVEL2, LEVEL3, LEVEL4, LEVEL5 };

	virtual bool DoCheckThreshold(uint32_t data_id, const std::string& key, float value, int signal_index = 1)
	{
		return false;
	}

	virtual bool DoBatchCheckThreshold(uint32_t data_id, const std::vector<rule_value>& ruleValueVec)
	{
		return false;
	}

	virtual bool AddAlert(unsigned int data_id,
	                      int level,
	                      const std::string& signal_name,
	                      const std::string& signal_id,
	                      const std::string& subject,
	                      uint16_t index = 1) = 0;
	virtual bool RestoreAlert(unsigned int data_id, int level, const std::string& signal_id, uint16_t index = 1) = 0;
	virtual bool HasAlert(unsigned int data_id, int level, const std::string& signal_id, uint16_t index = 1) = 0;

#ifndef SMDDEVICE
	virtual bool SaveMongoObject(const std::string& device_type, bsoncxx::builder::basic::document& doc)
	{
		return false;
	}
#endif
	virtual bool SaveDeviceHistory(uint32_t data_id, const std::string& dev_type, const uint8_t* data, size_t data_len)
	{
		return false;
	}
	virtual void SetDeviceThreshold(uint32_t data_id,
	                                const std::string& keyId,
	                                const std::string& type,
	                                int level,
	                                const std::string& signalId,
	                                const std::string& msg,
	                                float value) {};
	virtual void SetAlarmIdBDelay(uint32_t data_id, const std::string& alarmId, float bDelay) {};
	virtual void SetAlarmIdEDelay(uint32_t data_id, const std::string& alarmId, float eDelay) {};

	// Send Serial Port Data Down to remote board
	virtual bool OpenPort(uint32_t data_id, int baud_rate, int char_size = 8, int stop_bit_size = 1)
	{
		return false;
	}

	virtual bool ClosePort(uint32_t data_id)
	{
		return false;
	}
	virtual bool SendData(uint32_t data_id, const unsigned char* data, size_t len)
	{
		return SendParityData(data_id, data, len, NONE);
	}

	virtual bool SendDelayData(uint32_t data_id, int ms)
	{
		return false;
	}
	virtual bool SendParityData(uint32_t data_id, const unsigned char* data, size_t len, EnumParity type)
	{
		return false;
	}

	virtual void FlushCacheCmd()
	{
	}
	virtual bool SendDoData(int num, char data)
	{
		return false;
	}
	virtual int ReadDi(int index)
	{
		throw std::out_of_range("数据未就绪");
	}
	virtual float ReadAi(int index)
	{
		throw std::out_of_range("数据未就绪");
	}

	virtual float Get_SP_Value(unsigned int data_id, const std::string& var_name)
	{
		throw std::out_of_range("数据未就绪");
	}

	// Notify System of data we received
	virtual bool SendSP(unsigned int data_id, int c_type, const unsigned char* data, size_t len, bool bSave = false)
	{
		return false;
	}
	/// 这个因为要写到cache里，cache不支持vector const_buffer的写法，还得拷贝内存
	// virtual bool SendSP((unsigned int data_id, int c_type, const std::vector<boost::asio::const_buffer> &sendBuf,
	// const std::string &piJson = "") = 0; Save Data To Memcache
	virtual bool SaveCache(const std::string& key, const char* value, size_t len, unsigned int expiredSeconds = 60)
	{
		return false;
	}
	virtual bool SaveCache(const std::string& key, const std::string& value, unsigned int expiredSeconds = 60)
	{
		return false;
	}

	virtual bool GetCacheValue(const std::string& key, std::string& value)
	{
		return false;
	}

	virtual bool GetCacheInt(const std::string& key, int& iValue)
	{
		return false;
	}

	virtual void RTLog(const char* pFormat, ...);

	virtual void CheckThresholdWork(std::function<void()> f)
	{
	}

	void OpenLog(const std::string& file_name);

	template <typename... Args> void InfoLog(const char* fmt, const Args&... args)
	{
#ifndef SMDDEVICE
		if(spdloger_ != nullptr) {
			try {
				spdloger_->info(fmt, args...);
			} catch(const spdlog::spdlog_ex& ex) {
				std::cout << "InfoLog Exception:" << ex.what() << std::endl;
			}
		}
#endif
	}
	//提供默认实现，默认返回-1表示未实现，子类使用override标识
	virtual tcp::socket::native_handle_type StartSocket(uint32_t data_id, std::string ip, uint16_t port)
	{
		return -1;
	}
	virtual int CloseSocket(uint32_t data_id, tcp::socket::native_handle_type socket)
	{
		return -1;
	}
	virtual int SendSocketData(uint32_t data_id, tcp::socket::native_handle_type, const unsigned char* data, size_t len)
	{
		return -1;
	}
	virtual int SendSocketData(uint32_t data_id,
	                           tcp::socket::native_handle_type,
	                           const std::vector<boost::asio::const_buffer>& sendBuf)
	{
		return -1;
	}
	virtual int SendSocketData(uint32_t data_id, tcp::socket::native_handle_type, const std::string& sData)
	{
		return -1;
	}

private:
#ifndef SMDDEVICE
	std::shared_ptr<spdlog::logger> spdloger_;
	concurrent_unordered_map<std::string, bool> alertMap;
#else
	std::map<std::string, bool> alertMap;
#endif
};

// PLUMA_PROVIDER_HEADER(SMDController);

#endif
