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
            int r =  Send_Read_Property_Request_Address((BACNET_OBJECT_TYPE){{ sc.Cmd }}, v{{ sc.Cmd }}addr[i], PROP_PRESENT_VALUE);
            if(r){
                int len = Receive();
                if(len)
                {
                    {% if sc.Cmd == 3 or sc.Cmd == 4 or sc.Cmd == 5 %}
                    cData.v{{ sc.Cmd }}[i] = appValue.type.Boolean;
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


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back ( std::make_shared<{{ Project.Name }}Provider>() );
    return std::move ( providerVec );
}

#endif

