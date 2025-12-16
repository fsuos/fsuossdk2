#ifndef SMDVDEVICE_H
#define SMDVDEVICE_H
#include <json/json.h>
#include "SMDController.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <sstream>
#include "Provider.hpp"
//#include <oolua/oolua.h>
#include <map>

#include "VDeviceInterface.h"
#include <memory>
#ifdef ENABLE_RCF
#include <RCF/RCF.hpp>
#endif

#define MAXSAVETIMEINTERVAL   (8*3600)		//最大的保存时间8小时
#define MAX_POWERMETER_SAVEINTERVAL 600

class SMDVDevice : public VDeviceInterface, public std::enable_shared_from_this<SMDVDevice>
{
public:
	SMDVDevice();
	virtual ~SMDVDevice() {};

	virtual bool Reset() ;
	
	virtual bool InitV(SMDVDevice *vdevice, unsigned int  data_id, const Json::Value& settingRoot);
	virtual bool Init(std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot);
	virtual bool InitSetting(const Json::Value &settingRoot);
    
    void UpdateThresholdSetting(const Json::Value &settingRoot);
#ifdef ENABLE_RCF
	virtual bool InitRPC(boost::asio::io_context *network_io_service, std::shared_ptr<RcfClient<SMDControllerRPC> >  controller_rpc, unsigned int  data_id, const Json::Value& settingRoot);
#endif
    
    virtual int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned);

#ifndef SMDDEVICE
	bool SaveMongoObject(const std::string& device_type, bsoncxx::builder::basic::document &);
#endif
	//2022-06-19 张杨，规范保存历史数据的统计做法，因为服务器端用的是mongodb，FSU上用的是sqlite数据库，设备驱动里不再区分
	bool SaveDeviceHistory(const uint8_t *data, size_t data_len);
	
	virtual bool IsSupportDevice(const std::string& type) const;



	//virtual bool LoadPiSetting(const Json::Value& settingRoot);

	bool SaveCache(const std::string &key, const char*value, size_t len, unsigned int expiredSeconds = 60);
	bool SaveCache(const std::string &key, const std::string &value, unsigned int expiredSeconds = 60);

	bool GetCacheValue(const std::string &key, std::string &value);
	bool GetCacheInt(const std::string &key, int &iValue);

	void SetDeviceThreshold(const std::string& keyId, const std::string& type, int level, const std::string& signalId,  const std::string &msg, float value);
	void CheckThresholdWork(std::function<void()> f);

	virtual std::shared_ptr<SMDVDevice> GetChildVDevice(uint32_t data_id)
	{
		return nullptr;
	}
	virtual std::vector<std::shared_ptr<SMDVDevice> > GetAllChildVDevices()
	{
		return std::vector<std::shared_ptr<SMDVDevice> >();
	}
	void OpenLog();
#ifndef SMDDEVICE
	virtual void RTLog(const char* pFormat, ... );
#endif
	void SetAOAlarmRule(const std::string& aoSignalId, const Json::Value &setting);
	void SetAOReportSetting(const std::string& signal_id, const Json::Value& setting);
	void SetAlarmDelaySetting(const std::string& signal_id, const Json::Value& setting);

	bool CheckDIReport(const std::string& diSignalId);
	bool CheckAOReport(const std::string& aoSignalId, float value);
	struct BAOSetting {
		bool Threshold_valid = false;
		float Threshold;//变化阈值（模拟量变化阈值，超出主动上送）
		bool RelativeVal_valid = false;
		float RelativeVal;//变化百分比（变化阈值的百分比表示）
		bool IntervalTime_valid = false;
		float IntervalTime;//定时上送周期（单位：分）
		uint32_t lastUploadSecond;
		float         lastValue;
	};
	//signalId <-> rule
	std::map<std::string, BAOSetting> aoMap;


protected:
	uint16_t signal_index_;
	template <typename... Args>
	void InfoLog(const char* fmt, const Args&... args)
	{
		if(controller_.use_count()) {
			if(auto c = controller_.lock()) {
				c->InfoLog(fmt, args...);
			}
		} else {
#ifndef SMDDEVICE
			if(spdloger_ != nullptr) {
				try {
					spdloger_->info(fmt, args...);
				} catch(const spdlog::spdlog_ex &ex) {
					std::cout << "InfoLog Exception:" << ex.what() << std::endl;
				}
			}
#endif
		}
	}

	virtual bool SendSP(int c_type, const uint8_t *data, size_t len, bool bSave = false) final;
    virtual bool SendDelayData(int ms) final;
	virtual bool SendDoData(int num, char data) final;
	virtual int ReadDi(int index) final;
	virtual float ReadAi(int index) final;
	virtual float Get_SP_Value(unsigned int data_id, const std::string& var_name) final;
	//现场是batch模式

	void CheckThreshold(const std::string& key, const std::string& label, const float& value, int signal_index = 1);

	void CheckAlertMap(uint8_t value, int level, const std::string &key, const std::string &signal_id, const std::string &signal_name, const std::string &subject, const std::map<uint8_t, std::string>& rules, uint16_t signal_index = 1);

	void CheckThresholdBoolRule(int level, const std::string& key, uint8_t value, std::tuple<uint8_t,  std::string, std::string> rule, int signal_index = 1);

	void CheckThresholdBool(int level, const std::string& key, const std::string &signal_id, const std::string &signal_name, const std::string& subject, bool value, uint16_t signal_index = 1);

	bool DoBatchCheckThreshold(const std::vector<rule_value> &rvVec);

	//这种适用于DI/AI这种简单的，直接做判断
	bool DoCheckThreshold(const std::string& key, const float& value, int signal_index = 1);

	/*std::string category;//设备型号，比如48V电池，开关电源交流屏等
	std::string db_username;
	std::string db_password;
	std::string db_host;
	std::string db_host_readonly;
	std::string db_name;
	std::string mongodb_uri_;*/


	boost::posix_time::ptime lastTime;//最后依次发送实时数据的时间
	

	//Json::Value thresholdRoot;

	int index;

	unsigned int sample_index_;

	/// 这里主要防止，数据还没有采集到，而动态配置开始要数据进行判断
	bool bIsDataReady_;
	//pi related
	//std::string piJson;
	//void HandlePI();
	//bool hasPiFunction;
	//std::string piScript;
	//std::vector<std::string> piVec;
	//std::map<std::string, std::string> piMap;
	typedef struct pi_alert_ {
		std::string name;
		std::string label;
		int 		level;
		std::string signal_name;
		std::string signal_id;
		std::string  msg;
	} pi_alert;


	std::vector<rule_value> ruleValueVec;
	std::vector<boost::shared_ptr<pi_alert> > alertVec;
	//OOLUA::Script m_lua;
	boost::mutex luaMutex;
	std::map<std::string, bool> alertMap;
public:
	std::weak_ptr<SMDController> controller_;
#ifdef ENABLE_RCF
	std::shared_ptr<RcfClient<SMDControllerRPC> > controller_rpc_;

	boost::asio::io_context *p_io_service_ = nullptr;

#endif
#ifndef SMDDEVICE
protected:
	std::shared_ptr<spdlog::logger> spdloger_ = nullptr;
#endif
	int save_interval_ = MAXSAVETIMEINTERVAL;
	bool bSaveHistory = false;
	boost::posix_time::ptime lastSaveTime;	//数据保存时间点
	
    
	//告警缓存相关
    //是否激活，告警判断缓存,这个无论激活不激活，前端threshold里都用加个1s的定时器，来计算超时问题
    bool active_threshold_cache_ = true;
    //关于float判断的问题，以往要求2个float判断，要取fabs在0.01范围，实际上，咱们这种一个同一个数反复传入，而float本身也是4字节hex，所以直接做判断没问题
    std::map<std::string, float> ruleValueMap_;

	//联通B接口使用，AO，DO的信号列表,主要的困难点是有些数组类型的#需要自动替换，可以先不支持
	std::vector<std::string> aoVec_;
	std::map<std::string, float> aoValueMap_;
	std::vector<std::string> doVec_;
	std::vector<std::string> alarmVec_;
    //电信B接口相关-本设备使用的signals
    std::vector<std::string> signalsUsed_;
    std::map<std::string, int> levelMap_;   //告警等级
    std::map<std::string, float> valueMap_; //电信阈值

    bool bCompareSave = false;
};

#endif // SMDVDEVICE_H
