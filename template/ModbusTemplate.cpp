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
    {% for tsc in Sample %}
    {% if tsc.CmdGroupStart is defined %}               
        {% for sc in tsc.CmdGroupSample %}
        state = {{ Project.Name + "_R%d_%d_%d"%( sc.Cmd, tsc.CmdGroupStart, sc.Offset) + ";" }}
        cmdgroup_step_ = 0;
        {% if sc.Cmd == 3 %}
        modbus_read_registers({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
        {% elif sc.Cmd == 4 %}
        modbus_read_input_registers({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
        {% elif sc.Cmd == 1 %}
        modbus_read_bits({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
        {% elif sc.Cmd == 2 %}
        modbus_read_input_bits({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
        {% endif %}
        {% break %}
        {% endfor %}
        {% break %}
    {% else %}
        state = {{ Project.Name + "_R%d_%d"%(tsc.Cmd, tsc.Offset) + ";" }}
        {% if tsc.Delay is defined %}
        SendDelayData({{tsc.Delay}});
        {% endif %}
        {% if tsc.Cmd == 3 %}
        modbus_read_registers({{ tsc.Offset }}, {{ tsc.Len }});
        {% elif tsc.Cmd == 4 %}
        modbus_read_input_registers({{ tsc.Offset }}, {{ tsc.Len }});
        {% elif tsc.Cmd == 1 %}
        modbus_read_bits({{ tsc.Offset }}, {{ tsc.Len }});
        {% elif tsc.Cmd == 2 %}
        modbus_read_input_bits({{ tsc.Offset }}, {{ tsc.Len }});
        {% endif %}
        {% break %}
    {% endif %}
    {% endfor %}
    return true;
}

bool {{ Project.Name }}::process_payload(enum tab_type type, size_t len)
{
    switch(state){
    {% for tsc in Sample %}
    {% set tscNext = loop.nextitem %}
    {% if tsc.CmdGroupStart is defined %}               
    {% for sc in tsc.CmdGroupSample %}
        case {{ Project.Name + "_R%d_%d_%d"%(sc.Cmd, tsc.CmdGroupStart, sc.Offset) + ":" }}{
            //for($cgIndex = {{ tsc.CmdGroupStart }},$index = 1; $cgIndex < {{ tsc.CmdGroupEnd }}; $cgIndex+={{ tsc.CmdGroupStep}}, $index++){
            {% if sc.Cmd == 3 or sc.Cmd == 4 %}
                memcpy(cData.{{ "r%d_%d_%d[cmdgroup_step_]"%(sc.Cmd,tsc.CmdGroupStart,sc.Offset ) }}, tab_reg, sizeof(uint16_t)*{{ sc.Len }});
                {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
                memcpy(cData.{{ "b%d_%d_%d[cmdgroup_step_]"%(sc.Cmd,tsc.CmdGroupStart,sc.Offset) }}, tab_bit, sizeof(uint8_t)*{{  sc.Len }});
            {% endif %}
            {% if loop.nextitem is defined %}
                state = {{ Project.Name + "_R%d_%d_%d"%(loop.nextitem.Cmd, tsc.CmdGroupStart, loop.nextitem.Offset) + ";" }}
                {% if loop.nextitem.Delay is defined %}
                SendDelayData({{loop.nextitem.Delay}});
                {% endif %}
                {% if loop.nextitem.Cmd == 3 %}
                modbus_read_registers({{ tsc.CmdGroupStart + loop.nextitem.Offset }} + {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ loop.nextitem.Len }});
                {% elif loop.nextitem.Cmd == 4 %}
                modbus_read_input_registers({{ tsc.CmdGroupStart + loop.nextitem.Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ loop.nextitem.Len }});
                {% elif loop.nextitem.Cmd == 1 %}
                modbus_read_bits({{ tsc.CmdGroupStart + loop.nextitem.Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ loop.nextitem.Len }});
                {% elif loop.nextitem.Cmd == 2 %}
                modbus_read_input_bits({{ tsc.CmdGroupStart + loop.nextitem.Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ loop.nextitem.Len }});
                {% endif %}
            break;
        }
                {% else %}
                cmdgroup_step_++;
                if(cmdgroup_step_ == {{ "%d"%((tsc.CmdGroupEnd-tsc.CmdGroupStart)/tsc.CmdGroupStep) }})
                {
                    cmdgroup_step_ = 0;
                    {% if tscNext is defined %}
                        {% if tscNext.CmdGroupStart is defined %}               
                            {% for sc in tscNext.CmdGroupSample %}
                            state = {{ Project.Name + "_R%d_%d_%d"%(sc.Cmd, tscNext.CmdGroupStart, sc.Offset) + ";" }}
                            {% if sc.Delay is defined %}
                            SendDelayData({{sc.Delay}});
                            {% endif %}
                            cmdgroup_step_ = 0;
                            {% if sc.Cmd == 3 %}
                            modbus_read_registers({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                            {% elif sc.Cmd == 4 %}
                            modbus_read_input_registers({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                            {% elif sc.Cmd == 1 %}
                            modbus_read_bits({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                            {% elif sc.Cmd == 2 %}
                            modbus_read_input_bits({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                            {% endif %}
                            {% break %}
                            {% endfor %}
                            break;
                        {% else %}
                            state = {{ Project.Name + "_R%d_%d"%(tscNext.Cmd,tscNext.Offset) + ";" }}
                            {% if tscNext.Delay is defined %}
                            SendDelayData({{tscNext.Delay}});
                            {% endif %}
                            {% if tscNext.Cmd == 3 %}
                            modbus_read_registers({{ tscNext.Offset }}, {{ tscNext.Len }});
                            {% elif tscNext.Cmd == 4 %}
                            modbus_read_input_registers({{ tscNext.Offset }}, {{ tscNext.Len }});
                            {% elif tscNext.Cmd == 1 %}
                            modbus_read_bits({{ tscNext.Offset }}, {{ tscNext.Len }});
                            {% elif tscNext.Cmd == 2 %}
                            modbus_read_input_bits({{ tscNext.Offset }}, {{ tscNext.Len }});
                            {% endif %}
                        {% endif %}
                     {% else %}
                        RoundDone();
                        return false;
                    {% endif %}
                }else{
                    state = {{ Project.Name + "_R%d_%d_%d"%(tsc.CmdGroupSample[0].Cmd, tsc.CmdGroupStart, tsc.CmdGroupSample[0].Offset) + ";" }}
                    {% if tsc.CmdGroupSample[0].Delay is defined %}
                    SendDelayData({{tsc.CmdGroupSample[0].Delay}});
                    {% endif %}
                    {% if tsc.CmdGroupSample[0].Cmd == 3 %}
                    modbus_read_registers({{ tsc.CmdGroupStart + tsc.CmdGroupSample[0].Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ tsc.CmdGroupSample[0].Len }});
                    {% elif tsc.CmdGroupSample[0].Cmd == 4 %}
                    modbus_read_input_registers({{ tsc.CmdGroupStart + tsc.CmdGroupSample[0].Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ tsc.CmdGroupSample[0].Len }});
                    {% elif tsc.CmdGroupSample[0].Cmd == 1 %}
                    modbus_read_bits({{ tsc.CmdGroupStart + tsc.CmdGroupSample[0].Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ tsc.CmdGroupSample[0].Len }});
                    {% elif tsc.CmdGroupSample[0].Cmd == 2 %}
                    modbus_read_input_bits({{ tsc.CmdGroupStart + tsc.CmdGroupSample[0].Offset }}+ {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ tsc.CmdGroupSample[0].Len }});
                    {% endif %}
                }
        break;
        }
                {% endif %}
    {% endfor %}
    {% else %}
      case {{ Project.Name + "_R%d_%d"%(tsc.Cmd,tsc.Offset) + ":" }}{
	    {% if tsc.Cmd == 3 or tsc.Cmd == 4 %}
            memcpy(cData.{{ "r%d_%d"%(tsc.Cmd,tsc.Offset) }}, tab_reg, sizeof(uint16_t)*{{  tsc.Len }});
            {% elif tsc.Cmd == 1 or tsc.Cmd == 2 %}
	    memcpy(cData.{{ "b%d_%d"%(tsc.Cmd,tsc.Offset) }}, tab_bit, sizeof(uint8_t)*{{  tsc.Len }});
	    {% endif %}
	    {% if tscNext is defined %}
            {% if tscNext.CmdGroupStart is defined %}               
                {% for sc in tscNext.CmdGroupSample %}
                state = {{ Project.Name + "_R%d_%d_%d"%(sc.Cmd, tscNext.CmdGroupStart, sc.Offset) + ";" }}
                {% if sc.Delay is defined %}
                SendDelayData({{sc.Delay}});
                {% endif %}
                cmdgroup_step_ = 0;
                {% if sc.Cmd == 3 %}
                modbus_read_registers({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                {% elif sc.Cmd == 4 %}
                modbus_read_input_registers({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                {% elif sc.Cmd == 1 %}
                modbus_read_bits({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                {% elif sc.Cmd == 2 %}
                modbus_read_input_bits({{ tscNext.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                {% endif %}
                {% break %}
                {% endfor %}
            {% else %}
                state = {{ Project.Name + "_R%d_%d"%(tscNext.Cmd,tscNext.Offset) + ";" }}
                {% if tscNext.Delay is defined %}
                SendDelayData({{tscNext.Delay}});
                {% endif %}
                {% if tscNext.Cmd == 3 %}
                modbus_read_registers({{ tscNext.Offset }}, {{ tscNext.Len }});
                {% elif tscNext.Cmd == 4 %}
                modbus_read_input_registers({{ tscNext.Offset }}, {{ tscNext.Len }});
                {% elif tscNext.Cmd == 1 %}
                modbus_read_bits({{ tscNext.Offset }}, {{ tscNext.Len }});
                {% elif tscNext.Cmd == 2 %}
                modbus_read_input_bits({{ tscNext.Offset }}, {{ tscNext.Len }});
                {% endif %}
            {% endif %}
            break;
            {% else %}
            RoundDone();
            return false;
        {% endif %}
      }
      {% endif %}
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
    switch(ioControlCode) {
	case 310:{
	  //SET_AODATA
        std::string aoStr((char*)inBuffer, inBufferSize);
        Json::Value setting;//
        Json::Reader reader;//解析
        if(!reader.parse(aoStr, setting)) {
          return -1;
        }
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
            if(setting["signal_id"].asString() != "") {
                {% if SET_AO_CODE is defined %}
                {{ SET_AO_CODE }}
                {% endif %}
            }
        }
    }
    break;
    case 320: {
        state = 320;
        cmd_result_ = -1;
        OpenPort();
        //320是SET_DO
        std::string aoStr((char*)inBuffer, inBufferSize);
        Json::Value setting;//
        Json::Reader reader;//解析
        if(!reader.parse(aoStr, setting)) {
          return -1;
        }
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
            if(setting["signal_id"].asString() != "") {
                {% if SET_DO_CODE is defined %}
                {{ SET_DO_CODE }}
                {% endif %}
            }
        }
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
