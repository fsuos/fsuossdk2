#include "common_define.h"
#include "{{ Project.Name }}.h"
#include "UniDataDevice.cpp"
#include <future>

{{ Project.Name }}::{{ Project.Name }}()
{
    device_type_ = "{{ Project.Name|lower }}";
    addr_ = 1;

}

{{ Project.Name }}::~{{ Project.Name }}()
{
    if(nullptr != pCtx){
        modbus_close(pCtx);
        modbus_free(pCtx);
    }
}

bool {{ Project.Name }}::Init ( std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot )
{
    SMDSocketDevice::Init ( controller, data_id, settingRoot );
    {% if InitSetting is defined %}
    if(settingRoot["appSetting"] != Json::nullValue && settingRoot["appSetting"].type() == Json::objectValue) {
    {% for s in InitSetting %}
        if(settingRoot["appSetting"]["{{ s.Name }}"] != Json::nullValue) {
            {{s.Name}}_ = {% if s.Type == "int" %}atoi({%else%}atof({%endif%}settingRoot["appSetting"]["{{ s.Name }}"].asString().c_str());
        }
    {% endfor %}
    }
    {% endif %}
    return true;
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
   if(!ip_.empty())
    {
        if(state != IDLE)
        {
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            boost::posix_time::time_duration  diff = now - lastTime;
            if( diff.total_seconds() > 60) {
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
            }else{
                return false;
            }
        }
        state = 1;
        if(nullptr == pCtx){
            pCtx = modbus_new_tcp(ip_.c_str(), port_);
            std::async( std::launch::async,
                        [&]() {
                int nRet = modbus_connect(pCtx);
                if(-1 == nRet)
                {
                    //printf("connect failed\n");
                    modbus_close(pCtx);
                    modbus_free(pCtx);
                    pCtx = nullptr;
                    return;
                }
                uint16_t regs[125];
                uint8_t bits[2000];
                {% for sc in Sample %}
                    {% if sc.Cmd == 3 %}
                    if(-1 == modbus_read_registers(pCtx, {{ sc.Offset }}, {{ sc.Len }}, regs)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.r3_{{ sc.Offset }}, regs, sizeof(uint16_t)*{{ sc.Len }});
                    {% elif sc.Cmd == 4 %}
                    if(-1 == modbus_read_input_registers(pCtx, {{ sc.Offset }}, {{ sc.Len }}, regs)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.r4_{{ sc.Offset }}, regs, sizeof(uint16_t)*{{ sc.Len }});
                    {% elif sc.Cmd == 1 %}
                    if(-1 == modbus_read_bits(pCtx, {{ sc.Offset }}, {{ sc.Len }}, bits)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.b1_{{ sc.Offset }}, bits, {{ sc.Len }});
                    {% elif sc.Cmd == 2 %}
                    if(-1 == modbus_read_input_bits(pCtx, {{ sc.Offset }}, {{ sc.Len }}, bits)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.b2_{{ sc.Offset }}, bits, {{ sc.Len }});
                    {% endif %}                   
                {% endfor %}
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
                RoundDone();
                return;
            });
        }        
    }    
    return false;
}

bool {{ Project.Name }}::process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size)
{
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



int {{ Project.Name }}::DeviceIoControl(int ioControlCode, void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
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
    return UniDataDevice<{{ Project.Name }}_Data_t, SMDSocketDevice, RT_{{ Project.Name }}>::DeviceIoControl(ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned);
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
