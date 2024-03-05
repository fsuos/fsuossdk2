#include "SMDVDevice.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <fstream>
#include <future>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include "httptool.h"
extern int web_port;
SMDVDevice::SMDVDevice()
    : sample_index_(0)
    , controller_()
    , index(1)
    , bIsDataReady_(false)
    , signal_index_ { 1 }
{
}

bool SMDVDevice::InitV(SMDVDevice* vdevice, unsigned int data_id, const Json::Value& settingRoot)
{
    if(vdevice->controller_.use_count()) {
        return Init(vdevice->controller_, data_id, settingRoot);
    }
#ifdef ENABLE_RCF
    else {
        return InitRPC(vdevice->p_io_service_, vdevice->controller_rpc_, data_id, settingRoot);
    }
#endif
    return false;
}

#ifdef ENABLE_RCF
bool SMDVDevice::InitRPC(boost::asio::io_context* p_io_service,
    std::shared_ptr<RcfClient<SMDControllerRPC>> controller_rpc,
    unsigned int data_id,
    const Json::Value& settingRoot)
{
    p_io_service_ = p_io_service;
    controller_rpc_ = controller_rpc;
    Init(std::weak_ptr<SMDController>(), data_id, settingRoot);
    OpenLog();
    return true;
}
#endif

void SMDVDevice::UpdateThresholdSetting(const Json::Value& settingRoot)
{
    for(auto it = settingRoot.begin(); it != settingRoot.end(); it++) {
        const std::string& signalId = it.key().asString();
        const Json::Value& setting = settingRoot[signalId];
        if(signalId.length() == 12) {
            // 应该只是联通B接口在用
            // 如果是AO
            if(std::find(aoVec_.begin(), aoVec_.end(), signalId) != aoVec_.end() ||
                std::find(aoVec_.begin(), aoVec_.end(), signalId.substr(0, 9)) != aoVec_.end()) {
                // 上下限告警
                SetAOAlarmRule(signalId, setting);
                // 波动上报
                SetAOReportSetting(signalId, setting);
            }
//            // 如果是ALARMID，则需要进行BDelay和EDelay设定
//            if(std::find(alarmVec_.begin(), alarmVec_.end(), signalId) != alarmVec_.end() ||
//                std::find(alarmVec_.begin(), alarmVec_.end(), signalId.substr(0, 9)) != alarmVec_.end()) {
//                SetAlarmDelaySetting(signalId, setting);
//            }
        }
        // 电信B接口的告警规则
        //2024-02-02 张杨，不再判断signalsUsed_，因为网站下发B接口配置的时候，不会下发无用的配置
        //if(std::find(signalsUsed_.begin(), signalsUsed_.end(), signalId) != signalsUsed_.end()) {
            if(setting.type() == Json::objectValue) {
                if(setting["AlarmLevel"] != Json::nullValue &&
                    setting["AlarmLevel"].type() != Json::nullValue) { // 读电信配置方案告警等级
                    std::string sAlarmLevel = setting["AlarmLevel"].asString();
                    // boost::trim(sAlarmLevel);
                    levelMap_[signalId] = atoi(sAlarmLevel.c_str());
                } // 读电信配置方案告警等级

                if(setting["AlarmThreshold"] != Json::nullValue &&
                    setting["AlarmThreshold"].type() != Json::nullValue) { // 读电信配置方案告警阀值
                    std::string sAlarmThreshold = setting["AlarmThreshold"].asString();
                    // boost::trim(sAlarmLevel);
                    valueMap_[signalId] = atof(sAlarmThreshold.c_str());
                } // 读电信配置方案告警阀值
            }
            SetAlarmDelaySetting(signalId, setting);
        //}
    }
}

bool SMDVDevice::InitSetting(const Json::Value& settingRoot)
{
    // 联通B接口，AO/DO在ini里有配置，这个配置主要用在RoundDone前的配置和DeviceIoControl的命令
    // 实际使用中发现，ini文件实际只配置了AI，DI信号，其他的都没有配置，这个地方这么做，相当与增加了前后台
    // 人员配合的工作量，不如只麻烦后台人员，直接在驱动里写aoVec_,doVec_,alarmVec_
    /*std::string ini_file = "./ini/";
    ini_file += device_type_ + ".ini";
    if(access( ini_file.c_str(), F_OK ) != -1)
    {
            boost::property_tree::ptree config;
            read_ini(ini_file, config);
            for (auto& section : config) {
                    if(section.first == "AO"){
                            //这个地方，对AO信号的记录，主要是为了AO波动阈值,可以自动调用
                            //bChangedSave = CheckAOReport("281611001001", ((float)cData.temperature1) /100)||
    bChangedSave;
                            //parsed[section.first] = read_as<Object>(section.second);
                            std::cout<<"Found AO Config:"<<std::endl;
                            for(auto& v : section.second){
                                    std::cout<<v.first<<" and "<<v.second.data()<<std::endl;
                                    aoVec_.push_back(v.first);
                            }
                    }else if(section.first == "DO"){
                            //parsed[section.first] = read_as<Object>(section.second);
                            std::cout<<"Found DO Config:"<<std::endl;
                            for(auto& v : section.second){
                                    std::cout<<v.first<<" and "<<v.second.data()<<std::endl;
                                    doVec_.push_back(v.first);
                            }
                    }else if(section.first == "ALARM"){
                            //parsed[section.first] = read_as<Object>(section.second);
                            std::cout<<"Found DO Config:"<<std::endl;
                            for(auto& v : section.second){
                                    std::cout<<v.first<<" and "<<v.second.data()<<std::endl;
                                    alarmVec_.push_back(v.first);
                            }
                    }
            }
    }	*/
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
		if(settingRoot["appSetting"]["SaveInterval"] != Json::nullValue) {
			save_interval_ = atoi(settingRoot["appSetting"]["SaveInterval"].asString().c_str());
		}
	}
    if(settingRoot["thresholdSetting"] != Json::nullValue) {
        UpdateThresholdSetting(settingRoot["thresholdSetting"]);
    }
    return true;
}

void SMDVDevice::SetAOAlarmRule(const std::string& aoSignalId, const Json::Value& setting)
{
    // 联通B接口，AO信号的告警ID可以自行推导
    if(setting.type() == Json::objectValue) {
        if(setting["HLimit"] != Json::nullValue && setting["HLimit"].type() != Json::nullValue &&
            setting["HLimit"].asString() != "") {
            float HLimit = atof(setting["HLimit"].asString().c_str());
            std::string shSignalId = aoSignalId;
            shSignalId[3] = '2';
            shSignalId[7] = '0';
            shSignalId[8] = '3';
            SetDeviceThreshold(aoSignalId, "upper", SMDController::LEVEL2, shSignalId, "", HLimit);
        }
        if(setting["SHLimit"] != Json::nullValue && setting["SHLimit"].type() != Json::nullValue &&
            setting["SHLimit"].asString() != "") {
            float SHLimit = atof(setting["SHLimit"].asString().c_str());
            std::string shSignalId = aoSignalId;
            shSignalId[3] = '2';
            shSignalId[7] = '0';
            shSignalId[8] = '4';
            SetDeviceThreshold(aoSignalId, "upper", SMDController::LEVEL1, shSignalId, "", SHLimit);
        }
        if(setting["LLimit"] != Json::nullValue && setting["LLimit"].type() != Json::nullValue &&
            setting["LLimit"].asString() != "") {
            float LLimit = atof(setting["LLimit"].asString().c_str());
            std::string shSignalId = aoSignalId;
            shSignalId[3] = '2';
            shSignalId[7] = '0';
            shSignalId[8] = '1';
            SetDeviceThreshold(aoSignalId, "lower", SMDController::LEVEL2, shSignalId, "", LLimit);
        }
        if(setting["SLLimit"] != Json::nullValue && setting["SLLimit"].type() != Json::nullValue &&
            setting["SLLimit"].asString() != "") {
            float SLLimit = atof(setting["SLLimit"].asString().c_str());
            std::string shSignalId = aoSignalId;
            shSignalId[3] = '2';
            shSignalId[7] = '0';
            shSignalId[8] = '2';
            SetDeviceThreshold(aoSignalId, "lower", SMDController::LEVEL1, shSignalId, "", SLLimit);
        }
    }
}

void SMDVDevice::SetDeviceThreshold(const std::string& keyId,
    const std::string& type,
    int level,
    const std::string& signalId,
    const std::string& msg,
    float value)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock()) {
            c->SetDeviceThreshold(data_id_, keyId, type, level, signalId, msg, value);
        }
    }
}

void SMDVDevice::SetAOReportSetting(const std::string& signal_id, const Json::Value& setting)
{
    BAOSetting& aoSetting = aoMap[signal_id];
    // 保存上报相关
    if(setting["Threshold"] != Json::nullValue && setting["Threshold"].type() != Json::nullValue) {
        std::string sThreshold = setting["Threshold"].asString();
        boost::trim(sThreshold);
        if(sThreshold == "") {
            aoSetting.Threshold_valid = false;
            aoSetting.Threshold = {};
        } else {
            aoSetting.Threshold_valid = true;
            aoSetting.Threshold = atof(sThreshold.c_str());
        }
    }
    if(setting["RelativeVal"] != Json::nullValue && setting["RelativeVal"].type() != Json::nullValue) {
        std::string sRelativeVal = setting["RelativeVal"].asString();
        boost::trim(sRelativeVal);
        if(sRelativeVal == "") {
            aoSetting.RelativeVal_valid = false;
            aoSetting.RelativeVal = {};
        } else {
            aoSetting.RelativeVal_valid = true;
            aoSetting.RelativeVal = atof(sRelativeVal.c_str());
        }
    }
    if(setting["IntervalTime"] != Json::nullValue && setting["IntervalTime"].type() != Json::nullValue) {
        std::string sIntervalTime = setting["IntervalTime"].asString();
        boost::trim(sIntervalTime);
        if(sIntervalTime == "") {
            aoSetting.IntervalTime_valid = false;
            aoSetting.IntervalTime = {};
        } else {
            aoSetting.IntervalTime_valid = true;
            aoSetting.IntervalTime = atof(sIntervalTime.c_str());
        }
    }
}

void SMDVDevice::SetAlarmDelaySetting(const std::string& signal_id, const Json::Value& setting)
{
    // 告警延迟相关
    if(setting.type() == Json::objectValue) {
        if(setting["BDelay"] != Json::nullValue && setting["BDelay"].type() != Json::nullValue) {
            float bDelay = 0.0;
            std::string sBDelay = setting["BDelay"].asString();
            boost::trim(sBDelay);
            if(sBDelay != "") {
                bDelay = atof(sBDelay.c_str());
            }
            if(controller_.use_count()) {
                if(auto c = controller_.lock()) {
                    c->SetAlarmIdBDelay(data_id_, signal_id, bDelay);
                }
            }
        }
        if(setting["EDelay"] != Json::nullValue && setting["EDelay"].type() != Json::nullValue) {
            float eDelay = 0.0;
            std::string sEDelay = setting["EDelay"].asString();
            boost::trim(sEDelay);
            if(sEDelay != "") {
                eDelay = atof(sEDelay.c_str());
            }
            if(controller_.use_count()) {
                if(auto c = controller_.lock()) {
                    c->SetAlarmIdEDelay(data_id_, signal_id, eDelay);
                }
            }
        }
    }
}
bool SMDVDevice::Init(std::weak_ptr<SMDController> controller, unsigned int data_id, const Json::Value& settingRoot)
{
    controller_ = controller;
    data_id_ = data_id;

    /*if(settingRoot["db_host"] != Json::nullValue) {
        db_host = settingRoot["db_host"].asString();
    } else {
        db_host = "localhost";
    }
    if(settingRoot["db_host_readonly"] != Json::nullValue) {
        db_host_readonly = settingRoot["db_host_readonly"].asString();
    } else {
        db_host_readonly = "localhost";
    }
    if(settingRoot["db_password"] != Json::nullValue) {
        db_password = settingRoot["db_password"].asString();
    } else {
        db_password = "harbork";
    }
    if(settingRoot["db_username"] != Json::nullValue) {
        db_username = settingRoot["db_username"].asString();
    } else {
        db_username = "root";
    }
    if(settingRoot["db_name"] != Json::nullValue) {
        db_name = settingRoot["db_name"].asString();
    } else {
        db_name = "jim_monitor2";
    }

    if(settingRoot["mongodb_uri"] != Json::nullValue) {
        mongodb_uri_ = settingRoot["mongodb_uri"].asString();
    } else {
        mongodb_uri_ = "mongodb://localhost";
    }*/
    return true; // LoadPiSetting(settingRoot);
}

static int upload_di_(uint32_t data_id, const std::string& signal_id)
{
    std::stringstream ss;
    ss << "http://localhost:" << web_port << "/welcome/upload_di/data_id/" << data_id << "/signal_id/" << signal_id;
    std::cout << ss.str() << " start " << std::endl;
    //system(ss.str().c_str());
    HttpTool httpTool_;
    httpTool_.Get(ss.str());
    std::cout << ss.str() << " done " << std::endl;
    return 0;
}
bool SMDVDevice::CheckDIReport(const std::string& diSignalId)
{
    boost::async( boost::launch::async, std::bind(&upload_di_,data_id_, diSignalId));
    return true;
}

static int upload_ai_(uint32_t data_id, const std::string& signal_id, float v)
{
    std::stringstream ss;
    ss << "http://localhost:" << web_port << "/welcome/upload_ai/data_id/"
       << data_id << "/signal_id/" << signal_id << "/value/" << v;
    std::cout << ss.str() << " start " << std::endl;
    HttpTool httpTool_;
    httpTool_.Get(ss.str());
    std::cout << ss.str() << " done " << std::endl;
    return 0;
}
bool SMDVDevice::CheckAOReport(const std::string& aoSignalId, float value)
{
    bool bChanged = false;
    BAOSetting& aoSetting = aoMap[aoSignalId];
    //std::cout << "CheckAOReport signalId:" << aoSignalId << "last value:" << aoSetting.lastValue << " value:" << value<< std::endl;
    if(aoSetting.Threshold_valid) {
        //std::cout << "Threshold:" << aoSetting.Threshold << std::endl;
        if(value > (aoSetting.lastValue + aoSetting.Threshold) || value < (aoSetting.lastValue - aoSetting.Threshold)) {
            bChanged = true;
            //std::cout << "hit" << std::endl;
        }
    }
    if(aoSetting.RelativeVal_valid) {
        //std::cout << "Threshold:" << aoSetting.RelativeVal << std::endl;
        if(value > (aoSetting.lastValue * (100 + aoSetting.RelativeVal) / 100) ||
            value < (aoSetting.lastValue * (100 - aoSetting.RelativeVal) / 100)) {
            bChanged = true;
            //std::cout << "hit" << std::endl;
        }
    }
    if(aoSetting.IntervalTime_valid) {
        //std::cout << "Threshold:" << aoSetting.IntervalTime << std::endl;
        if(time(NULL) > (aoSetting.lastUploadSecond + 60 * aoSetting.IntervalTime)) {
            bChanged = true;
            //std::cout << "hit" << std::endl;
        }
    }

    if(bChanged) {
        //std::cout << "CheckAOReport signalId:" << aoSignalId << " value:" << value << std::endl;
        aoSetting.lastValue = value;
        aoSetting.lastUploadSecond = time(NULL);
        // Notify B 接口
        boost::async( boost::launch::async, std::bind(&upload_ai_,data_id_, aoSignalId, value));
        return true;
    }
    return false;
}

bool SMDVDevice::IsSupportDevice(const std::string& type) const
{
    if(boost::iequals(type, device_type_)) {
        return true;
    }
    return false;
}

void SMDVDevice::OpenLog()
{
    return;
#ifdef ENABLE_RCF
    std::string file_name = "rpc_" + boost::lexical_cast<std::string>(data_id_);
    std::string logFile = "logs/" + file_name;
    try {
        spdloger_ = spdlog::get(file_name);
        if(spdloger_ == nullptr) {
            spdloger_ = spdlog::rotating_logger_mt(file_name, logFile, 1048576 * 1, 1);
            spdloger_->set_level(spdlog::level::trace);
        }
    } catch(const spdlog::spdlog_ex& ex) {
        std::cout << "OpenLog" << file_name << " Exception:" << ex.what() << std::endl;
    }
#endif
}

void SMDVDevice::CheckThresholdWork(std::function<void()> f)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            c->CheckThresholdWork(f);
    }
#ifdef ENABLE_RCF
    else if(p_io_service_ != nullptr) {
        p_io_service_->post(f);
    }
#endif
    else {
        f();
    }
}

#ifndef SMDDEVICE
bool SMDVDevice::SaveMongoObject(const std::string& device_type, bsoncxx::builder::basic::document& doc)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            c->SaveMongoObject(device_type, doc);
    }
    // 不想支持rpc方式，那个要做的话就转成原始uint，然后再还原
    return false;
}
#endif

bool SMDVDevice::SaveDeviceHistory(const uint8_t* data, size_t data_len)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock()) {
            // 判断保存时间的得在这里做，避免每个驱动都自己判断
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            boost::posix_time::time_duration diff = now - lastSaveTime;
            std::cout<<"SMDVDevice::SaveDeviceHistory data_id:"<<data_id_<<" diff:"<<diff.total_seconds() << " save_interval_:"<<save_interval_<<std::endl;
            if((save_interval_ && diff.total_seconds() > save_interval_) || bSaveHistory || bCompareSave) {
                lastSaveTime = now;
                c->SaveDeviceHistory(data_id_, device_type_, data, data_len);
            }
        }
    }
    return false;
}

bool SMDVDevice::SendDoData(int num, char data)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SendDoData(num, data);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->SendDoData(num, data);
    }
#endif
    return false;
}

bool SMDVDevice::SendDelayData(int ms)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SendDelayData(data_id_, ms);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->SendDelayData(data_id_, ms);
    }
#endif
    return false;
}

int SMDVDevice::ReadDi(int index)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->ReadDi(index);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->ReadDi(index);
    }
#endif
    return -1;
}

float SMDVDevice::ReadAi(int index)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->ReadAi(index);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->ReadAi(index);
    }
#endif
    return 0.0;
}

float SMDVDevice::Get_SP_Value(unsigned int data_id, const std::string& var_name)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->Get_SP_Value(data_id, var_name);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->Get_SP_Value(data_id, var_name);
    }
#endif
    return 0.0;
}

bool SMDVDevice::SaveCache(const std::string& key, const char* value, size_t len, unsigned int expiredSeconds)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SaveCache(key, value, len, expiredSeconds);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        std::string sData(value, len);
        return controller_rpc_->SaveCache(key, sData, expiredSeconds);
    }
#endif
    return false;
}
bool SMDVDevice::SaveCache(const std::string& key, const std::string& value, unsigned int expiredSeconds)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SaveCache(key, value, expiredSeconds);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->SaveCache(key, value, expiredSeconds);
    }
#endif
    return false;
}

bool SMDVDevice::GetCacheValue(const std::string& key, std::string& value)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->GetCacheValue(key, value);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->GetCacheValue(key, value);
    }
#endif
    return false;
}

bool SMDVDevice::GetCacheInt(const std::string& key, int& iValue)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->GetCacheInt(key, iValue);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->GetCacheInt(key, iValue);
    }
#endif
    return false;
}

#ifndef SMDDEVICE
void SMDVDevice::RTLog(const char* pFormat, ...)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock()) {
            va_list args;
            va_start(args, pFormat);
            c->RTLog(pFormat, args);
            va_end(args);
        }
    }
}
#endif

bool SMDVDevice::SendSP(int c_type, const uint8_t* data, size_t len, bool bSave)
{
    lastTime = boost::posix_time::second_clock::local_time();
    if(controller_.use_count()) {
        if(auto c = controller_.lock())
            return c->SendSP(data_id_, c_type, data, len, bSave);
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        std::string tData((const char*)data, len);
        return controller_rpc_->SendSP(data_id_, c_type, tData, bSave);
    }
#endif
    return false;
}

void SMDVDevice::CheckThreshold(const std::string& key, const std::string& label, const float& value, int signal_index)
{
    //std::cout<<"SMDVDevice::CheckThreshold active_threshold_cache_"<<active_threshold_cache_<<std::endl;
    if(active_threshold_cache_) {
        // 这个缓存的风险之处在于，我不知道全局的告警规则会不会变化，现在就是要求，修改全局告警规则，要重新启动
        // 而通过B接口下发的规则，我们会自动做ruleValueMap_的清空（不需要锁，因为deviceiocontrol已经保证独占），这样就会重新使用缓存。
        std::string ruleKey = key + "_" + boost::lexical_cast<std::string>(signal_index);
        //std::cout<<"SMDVDevice::CheckThreshold ruleKey:"<<ruleKey<<std::endl;
        if(ruleValueMap_.find(ruleKey) == ruleValueMap_.end() || ruleValueMap_[ruleKey] != value) {
            ruleValueMap_[ruleKey] = value;
        } else {
            return;
        }
    }
    // std::cout<<"SMDVDevice::CheckThreshold before push"<<std::endl;
    rule_value newRV;
    newRV.isCheckBool = false;
    newRV.key = key;
    newRV.label = label;
    newRV.value = value;
    newRV.level = 0;
    newRV.signal_index = signal_index;
    ruleValueVec.push_back(newRV);
}

/*void SMDVDevice::CheckThreshold(const std::string& key, const std::string& alert_key, const  std::string& label, const
float& value)
{
        ruleValueVec.push_back(rule_value {false, key, alert_key, label, value});
}*/

void SMDVDevice::CheckAlertMap(uint8_t value,
    int level,
    const std::string& key,
    const std::string& signal_id,
    const std::string& signal_name,
    const std::string& subject,
    const std::map<uint8_t, std::string>& rules,
    uint16_t signal_index)
{
    auto it = rules.find(value);
    if(it != rules.end()) {
        if(it == rules.begin()) {
            CheckThresholdBool(level, key, signal_id, signal_name, subject + it->second, false, signal_index);
        } else {
            CheckThresholdBool(level, key, signal_id, signal_name, subject + it->second, true, signal_index);
        }
    }
}

void SMDVDevice::CheckThresholdBoolRule(int level,
    const std::string& key,
    uint8_t value,
    std::tuple<uint8_t, std::string, std::string> rule,
    int signal_index)
{
    CheckThresholdBool(level, key, std::get<1>(rule), boost::lexical_cast<std::string>((int)value), std::get<2>(rule),
        value == std::get<0>(rule), signal_index);
}

void SMDVDevice::CheckThresholdBool(int level,
    const std::string& key,
    const std::string& signal_id,
    const std::string& signal_name,
    const std::string& subject,
    bool value,
    uint16_t signal_index)
{
    if(active_threshold_cache_) {
        std::string ruleKey = boost::lexical_cast<std::string>(level) + "_" + key +
            boost::lexical_cast<std::string>(signal_id) + "_" + boost::lexical_cast<std::string>(signal_index);
        if(ruleValueMap_.find(ruleKey) == ruleValueMap_.end() || ruleValueMap_[ruleKey] != value) {
            ruleValueMap_[ruleKey] = value;
        } else {
            return;
        }
    }
    rule_value newRV;
    newRV.isCheckBool = true;
    newRV.key = key;
    newRV.label = subject;
    newRV.value = value ? 1.0f : 0.0f;
    newRV.level = level;
    newRV.signal_id = signal_id;
    newRV.signal_name = signal_name;
    newRV.signal_index = signal_index;
    ruleValueVec.push_back(newRV);
}

bool SMDVDevice::DoBatchCheckThreshold(const std::vector<rule_value>& rvVec)
{
    // ruleValueVec
    // 这个有个问题，在alert线程运行的时候，不能保证采集线程不修改ruleValueVec的值，但是又必须保证采集最快速度，所有调用这个前就做个拷贝
    if(controller_.use_count()) {
        if(auto c = controller_.lock()) {
            // std::cout<<"before call controller DoBatchCheckThreshold"<<std::endl;
            bool ret = c->DoBatchCheckThreshold(data_id_, rvVec);
            // std::cout<<"after call controller DoBatchCheckThreshold"<<std::endl;
            return ret;
            // bStoreData_ = c->CheckThresholdBool(data_id_, level, key, signal_id, signal_name, subject, value,
            // signal_index) || bStoreData_;
        }
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        bool ret = controller_rpc_->DoBatchCheckThreshold(data_id_, rvVec);
        return ret;
        // bStoreData_ = controller_rpc_->CheckThresholdBool(data_id_, level, key, signal_id, signal_name, subject,
        // value, signal_index) || bStoreData_;
    }
#endif
    return false;
}

bool SMDVDevice::DoCheckThreshold(const std::string& key, const float& value, int signal_index)
{
    if(controller_.use_count()) {
        if(auto c = controller_.lock()) {
            return c->DoCheckThreshold(data_id_, key, value, signal_index);
        }
    }
#ifdef ENABLE_RCF
    else if(controller_rpc_ != nullptr) {
        return controller_rpc_->DoCheckThreshold(data_id_, key, value, signal_index);
    }
#endif
    return false;
}

int SMDVDevice::DeviceIoControl(int ioControlCode,
    const void* inBuffer,
    int inBufferSize,
    void* outBuffer,
    int outBufferSize,
    int& bytesReturned)
{
    if(ioControlCode == 330 || ioControlCode == 310) {
        // 电信B接口方案配置下发
        std::string settingStr((char*)inBuffer, inBufferSize); //{"signal_id":"140800280010","SetValue":""}
        Json::Value settingRoot;
        Json::Reader reader; // 解析
        if(!reader.parse(settingStr, settingRoot)) {
            return -1;
        } // 防止错误
        UpdateThresholdSetting(settingRoot);
        ruleValueMap_.clear();
    }
    return 0;
}
