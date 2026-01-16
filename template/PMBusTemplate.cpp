#include "common_define.h"
#include "{{ Project.Name }}.h"
#include "UniDataDevice.cpp"
#include <boost/lexical_cast.hpp>

{{ Project.Name }}::{{ Project.Name }}()
{
    device_type_ = "{{ Project.Name|lower }}";
    {% if Project.BaudRate is defined %}
    baud_rate_ = {{ Project.BaudRate }};
    {% else %}
    baud_rate_ = 9600;
    {% endif %}
    {% if Project.StopBits is defined %}
    stop_bit_size_ = {{ Project.StopBits }};
    {% endif %}
    {% if Project.Parity is defined %}
    parity_ = {{ Project.Parity }};
    {% endif %}
    addr_ = 1;
    //save_interval_ = 600;
}

{{ Project.Name }}::~{{ Project.Name }}()
{
}


bool {{ Project.Name }}::InitSetting(const Json::Value &settingRoot)
{
    cData.data_id = data_id_;
    {% if InitSetting is defined %}
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
    {% for s in InitSetting %}
        if(settingRoot["appSetting"]["{{ s.Name }}"] != Json::nullValue) {
            {{s.Name}}_ = {% if s.Type == "int" %}atoi({%else%}atof({%endif%}settingRoot["appSetting"]["{{ s.Name }}"].asString().c_str());
            {% if s.Min is defined %}
            if({{s.Name}}_ < {{s.Min}}){{s.Name}}_ = {{s.Min}};
            {% endif %}
            {% if s.Max is defined %}
            if({{s.Name}}_ > {{s.Max}}){{s.Name}}_ = {{s.Max}};
            {% endif %}
        }
    {% endfor %}
    }
    {% endif %}
    return UniDataDevice<{{ Project.Name }}_Data_t, PMBusProtocol, RT_{{ Project.Name }}>::InitSetting(settingRoot);
}

static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

void {{ Project.Name }}::RunCheckThreshold()
{    
    switch(b_mode_)
    {
        case 1://联通
        {
            {% if RunCheckThresholdCodeUnicom is defined %}{{ RunCheckThresholdCodeUnicom }}{% endif %}
            
            uint8_t* pCData = NULL;
            {% for tsc in Sample %}
              pCData = cData.{{  "r%d_%d_%d"%(tsc.CID1,tsc.CID2, tsc.InfoLabel) }};
              {% for d in tsc.Data %}
                {% if d.UnicomSignalId is defined %}
                {
                    {% if d.Type == 'f' %}
                    float tData;
                    memcpy(&tData, pCData + {{ d.Offset }}, 4);
                    {% elif d.Type == 'S' %}
                    uint16_t tData;
                    memcpy(&tData, pCData + {{ d.Offset }}, 2);
                    tData = ntohs(tData);
                    {% elif d.Type == 'S' %}
                    int16_t tData;
                    memcpy(&tData, pCData + {{ d.Offset }}, 2);
                    tData = ntohs(tData);
                    {% elif d.Type == 'C' %}
                    uint8_t tData;
                    memcpy(&tData, pCData + {{ d.Offset }}, 1);
                    {% endif %}
                    {% if d.UnicomSignalId|string|length == 12 %}
                        {% if d.AlertNormalValue is defined %}
                        CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  tData != {{ d.AlertNormalValue }}, signal_index_++); 
                        {% elif d.AlertAbnormalValue is defined %}
                        CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  tData == {{ d.AlertAbnormalValue }}, signal_index_++); 
                        {% endif %}
                    {% else %}
                    {
                        std::stringstream ss;
                        ss<<"{{ d.UnicomSignalId }}" << std::setw(3)<<std::setfill('0')<<signal_index_;
                        {% if d.AlertNormalValue is defined %}
                        CheckThresholdBool(2, ss.str(), ss.str(), {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", tData != {{ d.AlertNormalValue }}, signal_index_++); 
                        {% elif d.AlertAbnormalValue is defined %}
                        CheckThresholdBool(2, ss.str(), ss.str(), {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", tData == {{ d.AlertAbnormalValue }}, signal_index_++); 
                        {% endif %}
                    }
                    {% endif %}
                }
                {% endif %}
                {% endfor %}   
            {% endfor %}
            break;
        }
        case 2://电信
        {
             {% if RunCheckThresholdCodeTelecom is defined %}{{ RunCheckThresholdCodeTelecom }}{% endif %}
             uint8_t* pCData = (uint8_t*)&cData;
            int offset = 4;
            {% for tsc in Sample %}
                 
            {% endfor %}
            break;
        }
        case 3://移动
        {
            {% if RunCheckThresholdCodeMobile is defined %}{{ RunCheckThresholdCodeMobile }}{% endif %}
            uint8_t* pCData = (uint8_t*)&cData;
            int offset = 4;
            {% for tsc in Sample %}
                
            {% endfor %}
            break;
        }
        default:{//默认电信动环的情况
            {% if RunCheckThresholdCodePTelecom is defined %}{{ RunCheckThresholdCodePTelecom }}{% endif %}
            uint8_t* pCData = (uint8_t*)&cData;
            int offset = 4;
            {% for tsc in Sample %}
                
            {% endfor %}
            break;
        }
    }
}

bool {{ Project.Name }}::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    {% for tsc in Sample %}
    
        state = {{ Project.Name + "_R%d_%d_%d"%(tsc.CID1, tsc.CID2, tsc.InfoLabel) + ";" }}
        {% if tsc.Info is defined %}
        uint8_t info[{{ tsc.InfoLen }}] = { {{ tsc.Info }}};
        int infoLen = {{ tsc.InfoLen }};
        write_pmbus_cmd ( {{tsc.CID1 }}, {{tsc.CID2}}, info, infoLen);
        {% else %}
        write_pmbus_cmd ( {{tsc.CID1 }}, {{tsc.CID2}});
        {% endif %}
        {% break %}
    {% endfor %}
    return true;
}

bool {{ Project.Name }}::process_payload()
{
    switch(state){
    {% for tsc in Sample %}
    {% set tscNext = loop.nextitem %}
      case {{ Project.Name + "_R%d_%d_%d"%(tsc.CID1,tsc.CID2,tsc.InfoLabel) + ":" }}{	    
        memcpy(cData.{{ "r%d_%d_%d"%(tsc.CID1,tsc.CID2, tsc.InfoLabel) }}, pPMBus->data, {{  tsc.Len }});            
	    {% if tscNext is defined %}
            state = {{ Project.Name + "_R%d_%d_%d"%(tscNext.CID1, tscNext.CID2,tscNext.InfoLabel) + ";" }}
            {% if tscNext.Info is defined %}
            uint8_t info[{{ tscNext.InfoLen }}] = { {{ tscNext.Info }}};
            int infoLen = {{ tscNext.InfoLen }};
            write_pmbus_cmd ( {{tscNext.CID1 }}, {{tscNext.CID2}}, info, infoLen);
            {% else %}
            write_pmbus_cmd ( {{tscNext.CID1 }}, {{tscNext.CID2}});
            {% endif %}
            break;
            {% else %}
            RoundDone();
            return false;
        {% endif %}
      }
      {% endfor %}
      {% if SET_RET_CODE is defined %}
      {{ SET_RET_CODE }}
      {% endif %}
   }
   return true;
}


float {{ Project.Name }}::Get_Value(uint32_t data_id, const std::string& var_name) const
{
    if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }
   {% if Value is defined %}
   {% for v in Value %}
   {% if loop.index == 1 %}
   if(var_name == "{{v.Name}}"){
   {% else %}
   else if(var_name == "{{v.Name}}"){
   {% endif %}
     return {{v.Value}};
   }
   {% endfor %}
   {% endif %}
    throw std::out_of_range("不支持变量");
}



int {{ Project.Name }}::DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
{
    std::cout<<"{{ Project.Name }}::DeviceIoControl "<<ioControlCode<<std::endl;
    if(ioControlCode%2 == 0 && state != IDLE)
    {
        std::cout<<"{{ Project.Name }}::DeviceIoControl work inprogress"<<std::endl;
        return -1;
    }
    switch(ioControlCode) {
	case 310:{

        state = 310;
        cmd_result_ = -1;
        OpenPort();
	  //SET_AODATA
        std::string aoStr((char*)inBuffer, inBufferSize);
        Json::Value setting;//
        Json::Reader reader;//解析
        if(!reader.parse(aoStr, setting)) {
          return -1;
        }
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
                {% if SET_AO_CODE is defined %}
                {{ SET_AO_CODE }}
                {% endif %}
        }
    }
    break;
    case 320: {
        state = 320;
        cmd_result_ = -1;
        OpenPort();
        //320是SET_DO
        {% if SET_DO_CODE is defined %}
        {{ SET_DO_CODE }}
        {% endif %}
        break;
    }
    case 801:    
    case 803:
    {
        //get fsuos json data
        {
            std::lock_guard<std::mutex> lLock(jsonValueMutex);
            if(lastTime != lastJsonTime_)
            {
                lastJsonTime_ = lastTime;
                uint8_t* pCData = (uint8_t*)&cData;
                int offset = 4;
                {% for tsc in Sample %}
                    
                {% endfor %}
                
                std::stringstream uSS;
                uSS<<(int)cData.update_time.year<<"-"<<(int)cData.update_time.month<<"-"<<(int)cData.update_time.day<<" "<<(int)cData.update_time.hour<<":"<<(int)cData.update_time.minute<<":"<<(int)cData.update_time.second;
                jsonValue["更新时间"] = uSS.str();
            }
        }
        if(ioControlCode == 801)
        {
            Json::FastWriter writer;
            std::string jsonStr = writer.write(jsonValue);
            if(outBufferSize >= jsonStr.size())
            {
                memcpy(outBuffer, jsonStr.data(), jsonStr.size());
                bytesReturned = jsonStr.size();
                return 0;
            }else{
                bytesReturned = jsonStr.size();
                return -1;
            }
        }else if(ioControlCode == 803){
            //绝大部分都会走这里，几乎不会走801
            if(outBufferSize >= sizeof(void*))
            {
                void *pJsonValue = &jsonValue;
                memcpy(outBuffer, &pJsonValue, sizeof(void*));
                bytesReturned = sizeof(void*);
                return 0;
            }else{
                bytesReturned = sizeof(void*);
                return -1;
            }
        }
        return 0;
    }
    {% if DEVICEIOCONTROL_CODE is defined %}
    {{ DEVICEIOCONTROL_CODE }}
    {% endif %}
    default:
        if((uint32_t)outBufferSize >= sizeof(int)) {
            *((int*)outBuffer) = cmd_result_;//无效命令
            bytesReturned = sizeof(int);
        } else {
            bytesReturned = 0;
        }
        break;
    }
    return UniDataDevice<{{ Project.Name }}_Data_t, PMBusProtocol, RT_{{ Project.Name }}>::DeviceIoControl(ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned);
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<{{ Project.Name }}Provider>());
    return std::move(providerVec);
}

#endif
