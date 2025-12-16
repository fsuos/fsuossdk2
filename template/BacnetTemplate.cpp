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
}


bool {{ Project.Name }}::RefreshStatus()
{
    std::cout<<"{{ Project.Name }}::RefreshStatus"<<std::endl;
    if(!ip_.empty())
    {
        {% for sc in Sample %}
        int v{{ sc.Cmd }}addr[] = { {{ sc.Offset|join(",")}} };
        for(int i=0; i< {{ sc.Offset|length }};i++)
        {
            boost::mutex::scoped_lock spLock(socket_mutex_);
	    {% if sc.Ip is defined %}
	    remote_endpoint_.address(boost::asio::ip::address_v4::from_string("{{ sc.Ip }}"));
	    {% endif %}
            int r =  Send_Read_Property_Request_Address((BACNET_OBJECT_TYPE){{ sc.Cmd }}, v{{ sc.Cmd }}addr[i], PROP_PRESENT_VALUE);
            if(r){
                int len = Receive();
                if(len)
                {
                    {% if sc.Cmd == 3 or sc.Cmd == 4 or sc.Cmd == 5 %}
                    cData.v{{ sc.Cmd }}[i] = appValue.type.Enumerated;
                    {% elif sc.Cmd == 0 or sc.Cmd == 1 or sc.Cmd == 2 %}
                    cData.v{{ sc.Cmd }}[i] = appValue.type.Real;
                    {% elif sc.Cmd == 19 %}
                    cData.v{{ sc.Cmd }}[i] = appValue.type.Signed_Int;
                    {% endif %}
                    std::cout<<ip_<<" get {{ sc.Cmd }} "<<v{{ sc.Cmd }}addr[i]<<" value:"<<cData.v{{ sc.Cmd }}[i]<<std::endl;

                }else{
                    std::cout<<"receive failed"<<std::endl;
                    return false;
                }
            }else{
                std::cout<<"send failed"<<std::endl;
                return false;
            }
        }
        {% endfor %}
        RoundDone();
    }    
    return false;
}

bool {{ Project.Name }}::process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size)
{
        return false;
}

void {{ Project.Name }}::RunCheckThreshold()
{
    switch(b_mode_)
    {
        case 1://联通
        {
            {% for sc in Sample %}
            {% for d in sc.Data %}
            {% if d.AlertNormalValue is defined or d.AlertAbnormalValue is defined %}
             {% if d.UnicomSignalId is defined %}
                {% if d.AlertNormalValue is defined %}
                CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{d.Name}}"{% endif %}, "{{d.Name}}",  (cData.v{{ sc.Cmd }}[{{ loop.index-1 }}]) != {{ d.AlertNormalValue }}, signal_index_++); 
                {% elif d.AlertAbnormalValue is defined %}
                CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{d.Name}}"{% endif %}, "{{d.Name}}",  (cData.v{{ sc.Cmd }}[{{ loop.index-1 }}]) == {{ d.AlertAbnormalValue }}, signal_index_++); 
                {% endif %}
             {% endif %}
            {% endif %}
            {% endfor %}
            {% endfor %}
            break;
        }
        case 2://电信
        {
            {% for sc in Sample %}
            {% for d in sc.Data %}
            {% if d.AlertNormalValue is defined or d.AlertAbnormalValue is defined %}            
                {% if  d.TeleSignalId is defined %}
                    {% if d.TeleSignalId|string|length == 12 %}
                        {% if d.AlertNormalValue is defined %}
                        CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{d.Name}}"{% endif %}, "{{d.Name}}",  (cData.v{{ sc.Cmd }}[{{ loop.index-1 }}]) != {{ d.AlertNormalValue }}, signal_index_++); 
                        {% elif d.AlertAbnormalValue is defined %}
                        CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{d.Name}}"{% endif %}, "{{d.Name}}",  (cData.v{{ sc.Cmd }}[{{ loop.index-1 }}]) == {{ d.AlertAbnormalValue }}, signal_index_++); 
                        {% endif %}
                    {% else %}
                    {
                        std::stringstream ss;
                        ss<<"{{ d.TeleSignalId }}" << std::setw(3)<<std::setfill('0')<<signal_index_<<"0";
                        {% if d.AlertNormalValue is defined %}
                        CheckThresholdBool(2, ss.str(), ss.str(), {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{d.Name}}"{% endif %}, "{{d.Name}}", (cData.v{{ sc.Cmd }}[{{ loop.index-1 }}]) != {{ d.AlertNormalValue }}, signal_index_++); 
                        {% elif d.AlertAbnormalValue is defined %}
                        CheckThresholdBool(2, ss.str(), ss.str(), {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{d.Name}}"{% endif %}, "{{d.Name}}", (cData.v{{ sc.Cmd }}[{{ loop.index-1 }}]) == {{ d.AlertAbnormalValue }}, signal_index_++); 
                        {% endif %}
                    }
                    {% endif %}
                {% endif %}
            {% endif %}
            {% endfor %}
            {% endfor %}
            break;
        }
    }    
}
#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back ( std::make_shared<{{ Project.Name }}Provider>() );
    return std::move ( providerVec );
}

#endif

