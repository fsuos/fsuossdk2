#include "common_define.h"
#include "{{ Project.Name }}.h"
#include "UniDataDevice.cpp"


{{ Project.Name }}::{{ Project.Name }}()
{
    device_type_ = "{{ Project.Name|lower }}";
    {% if Project.BaudRate is defined %}
    baud_rate_ = {{ Project.BaudRate }};
    {% else %}
    baud_rate_ = 9600;
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
        }
    {% endfor %}
    }
    {% endif %}
    return UniDataDevice<{{ Project.Name }}_Data_t, SPModbus, RT_{{ Project.Name }}>::InitSetting(settingRoot);
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
            break;
        }
        case 2://电信
        {
             {% if RunCheckThresholdCodeTelecom is defined %}{{ RunCheckThresholdCodeTelecom }}{% endif %}
            break;
        }
    }
}

bool {{ Project.Name }}::RefreshStatus()
{
    SMDSPDevice::RefreshStatus();
    {% for sc in Sample %}
    state = {{ Project.Name + "_R%d_%d"%(sc.Cmd, sc.Offset) + ";" }}
    {% if sc.Cmd == 3 %}
    modbus_read_registers({{ sc.Offset }}, {{ sc.Len }});
    {% elif sc.Cmd == 4 %}
    modbus_read_input_registers({{ sc.Offset }}, {{ sc.Len }});
    {% elif sc.Cmd == 1 %}
    modbus_read_bits({{ sc.Offset }}, {{ sc.Len }});
    {% elif sc.Cmd == 2 %}
    modbus_read_input_bits({{ sc.Offset }}, {{ sc.Len }});
    {% endif %}
    {% break %}
    {% endfor %}
    return true;
}

bool {{ Project.Name }}::process_payload(enum tab_type type, size_t len)
{
    switch(state){
    {% for sc in Sample %}
      case {{ Project.Name + "_R%d_%d"%(sc.Cmd,sc.Offset) + ":" }}{
	    {% if sc.Cmd == 3 or sc.Cmd == 4 %}
            memcpy(cData.{{ "r%d_%d"%(sc.Cmd,sc.Offset) }}, tab_reg, sizeof(uint16_t)*{{  sc.Len }});
            {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
	    memcpy(cData.{{ "b%d_%d"%(sc.Cmd,sc.Offset) }}, tab_bit, sizeof(uint8_t)*{{  sc.Len }});
	    {% endif %}
	    {% if loop.nextitem is defined %}
            state = {{ Project.Name + "_R%d_%d"%(loop.nextitem.Cmd,loop.nextitem.Offset) + ";" }}
    	    {% if loop.nextitem.Cmd == 3 %}
            modbus_read_registers({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }});
            {% elif loop.nextitem.Cmd == 4 %}
            modbus_read_input_registers({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }});
            {% elif loop.nextitem.Cmd == 1 %}
            modbus_read_bits({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }});
            {% elif loop.nextitem.Cmd == 2 %}
            modbus_read_input_bits({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }});
            {% endif %}
	    break;
            {% else %}
            RoundDone();
	    return false;
            {% endif %}
      }
      {% endfor %}
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
    switch(ioControlCode) {
	case 310:{
	  //SET_AODATA
          std::string aoStr((char*)inBuffer, inBufferSize);
          Json::Value setting;//
          Json::Reader reader;//解析
          if(!reader.parse(aoStr, setting)) {
            return -1;
          }
          for(auto it = setting.begin(); it != setting.end(); it++) {
            if((*it)["signal_id"] != Json::nullValue && (*it)["signal_id"].type()  != Json::nullValue) {
                if((*it)["signal_id"].asString() != "") {
                    SetAOAlarmRule((*it)["signal_id"].asString(), (*it));
		    {% if AO is defined %}
                    {% for ao in AO %}    
                    {% if loop.index == 1 %}
		    if((*it)["signal_id"].asString() == "{{ ao.SignalId }}"){
                    {% else %}
		    else if((*it)["signal_id"].asString() == "{{ ao.SignalId }}"){
		    {% endif %}
                      //{{ao.Desc}}
		      {% if ao.SetValue is defined and ao.SetValue == True %}
		      if(setting["SetValue"] != Json::nullValue && setting["SetValue"].type()  != Json::nullValue) {
		      }
		      {% endif %}
                      SetAOReportSetting("{{ ao.SignalId }}", setting);
                    }
		    {% endfor %}
		    {% endif %}
                }
            }
        }
    }
    break;
    case 320: {
        state = 320;
        cmd_result_ = -1;
        OpenPort();
        //320是SET_DO
        std::string doId((char*)inBuffer, inBufferSize);
        {% if DO is defined %}
        {% for do in DO %}    
	{% if loop.index == 1 %}
	if(doId == "{{ do.SignalId }}"){
	{% else %}
	else if(doId == "{{ do.SignalId }}"){
	{% endif %}
	//{{do.Desc}}
	}
	{% endfor %}
	{% endif %}
        break;
    }

    default:
        if((uint32_t)outBufferSize >= sizeof(int)) {
            *((int*)outBuffer) = 2;//无效命令
            bytesReturned = sizeof(int);
        } else {
            bytesReturned = 0;
        }
        break;
    }
    return UniDataDevice<{{ Project.Name }}_Data_t, SPModbus, RT_{{ Project.Name }}>::DeviceIoControl(ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned);
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
