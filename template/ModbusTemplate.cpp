#include "common_define.h"
#include "{{ Project.Name }}.h"
#include "UniDataDevice.cpp"
#include <boost/lexical_cast.hpp>
{% if BlockTemplate is defined %}
{% for key,blockDef in BlockTemplate.items() %}
void {{ Project.Name }}::_{{ Project.Name|lower }}_{{ key }}(char* pCData,const char* prefix, int iIndex, const char* index {% if blockDef.HasIndex1 is defined  %}, const char* index1{% endif %} {% if blockDef.HasIndex2 is defined %}, const char* index2{% endif %})
{
          int offset = 0;
    {% if blockDef.BlockType is defined %}
        {% if blockDef.BlockType == 'f' %}
        float pData[{{ (blockDef.BlockLength/4)|int }}];     
        {% elif blockDef.BlockType == 'S' %}
        uint16_t pData[{{ (blockDef.BlockLength/2)|int }}];
        {% elif blockDef.BlockType == 's' %}
        int16_t pData[{{ (blockDef.BlockLength/2)|int }}];
        {% elif blockDef.BlockType == "I" %}
        uint32_t pData[{{ (blockDef.BlockLength/4)|int }}];
        {% elif blockDef.BlockType == "i" %}
        int32_t pData[{{ (blockDef.BlockLength/4)|int }}];
        {% else %}
        uint16_t pData[{{ (blockDef.BlockLength/2)|int }}];
        {% endif %}
        memcpy(pData, pCData, {{blockDef.BlockLength}});
    {% else %}
    char * pData = pCData;
    {% endif %}

    {% for d in blockDef.BlockContent %}

      {% if d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}(pData + offset , prefix, iIndex, {% if d.index is defined %}"{{ d.index }}"{% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %}{% else %}index{% endif %});
      offset += {{ BlockTemplate[d.Block]["BlockLength"] }};
      {% else %}
        {
        char nameBuffer[48] = {0};
        {% if d.CIndex is defined %}
        snprintf(nameBuffer, 48, "{{ d.Name }}", {{ d.CIndex }});
        {% else %}
        snprintf(nameBuffer, 48, "{{ d.Name }}", {% if d.Index is defined %}{{ d.Index }}{% else %}index{% endif %});
        {% endif %}
        std::string name = nameBuffer;
        {% if d.CValue is defined %}
          {% if d.AlertNormalValue is defined %}
          jsonValue[name] = ({{ d.CValue }} == {{ d.AlertNormalValue }}) ? "正常" : "告警";
          jsonValue["AlertArray"][name] = ({{ d.CValue }} != {{ d.AlertNormalValue }});
          {% elif d.Options is defined %}
          switch({{ d.CValue }}){
          {% for item in d.Options %}
            case {{ item.Key }}:
            jsonValue[name] = "{{ item.Value }}";
              break;
          {% endfor %}
            default:
            jsonValue[name]  = "无效值";
              break;
          }
          {% else %}
          jsonValue[name]  = {{ d.CValue }};
          {% endif %}
        {% elif d.Options is defined %}
          switch(pData[0]){
          {% for item in d.Options %}
            case {{ item.Key }}:
            jsonValue[name]  = "{{ item.Value }}";
              break;
          {% endfor %}
            default:
              jsonValue[name]  = "无效值";
              break;
          }
        {% elif d.AlertNormalValue is defined %}
          jsonValue[name] = (pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] == {{ d.AlertNormalValue }}) ? "正常" : "告警";
          jsonValue["AlertArray"][name] = (pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }});
        {% elif d.ArrayName is defined %}
        for(int i=1;int i<={{ d.ArrayLength }};int i++){
          $name = $prefix.sprintf("{{ d.ArrayName }}", {{ d.ArrayStart }} + $i);
          int kIndex = {{ d.Offset }} + i;
          jsonValue[name] = ((float)pData[$kIndex-1]){% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %};
        }
        {% else %}
          jsonValue[name] = ((float)pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}]){% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %};
        {% endif %}
        }
      {% endif %}
          {% if d.Alias is defined %}
          {% for newName in d.Alias %}
          jsonValue["{{ newName }}"] = jsonValue["{{ d.Name }}"];
          {% endfor %}
          {% endif %}
    {% endfor %}
}
{% endfor %}
{% endif %}

{% macro render_sc_alert(sc, scPrefix=none) -%}
  {
  {% if sc.Data is defined %}
        {% set hasAlert = namespace(found=false) %}
        {% for d in sc.Data %}
           {% if d.AlertNormalValue is defined and d.TeleSignalId is defined  %}
            {% set hasAlert.found = true %}
           {% endif %}
        {% endfor %}
        {% if not hasAlert.found %}
         {% if sc.Cmd == 3 or sc.Cmd == 4 %}
        offset += {{ 2*sc.Len }};
        {% else %}
        offset += {{ sc.Len }};
        {% endif %}
        {% else %}
        {% if sc.Type is defined %}
            {% if sc.Type == 'f' %}
            float pData[{{ (sc.Len*2/4)|int }}];     
            {% elif sc.Type == 'S' %}
            uint16_t pData[{{ sc.Len }}];
            {% elif sc.Type == 's' %}
            int16_t pData[{{ sc.Len }}];
            {% elif sc.Type == "I" %}
            uint32_t pData[{{ (sc.Len*2/4)|int }}];
            {% elif sc.Type == "i" %}
            int32_t pData[{{ (sc.Len*2/4)|int }}];
            {% endif %}
        {% elif sc.Cmd == 3 or sc.Cmd == 4 %}
        uint16_t pData[{{ sc.Len }}];
        memcpy(pData, pCData + offset, {{2*sc.Len}});
        {% else %}
        uint8_t pData[{{ sc.Len }}];
        memcpy(pData, pCData + offset, {{sc.Len}});
        {% endif %}
        
	    {% for d in sc.Data %}
          {% if d.ArrayName is defined %}
          for(int i=1;i<={{ d.ArrayLength }};i++){
                char nameBuffer[48] = {0};
                snprintf(nameBuffer, 48, "{{ d.ArrayName }}", {{ d.ArrayStart }} + i);
                std::string name = nameBuffer;
                int kIndex = {{ d.Offset }} + i;
                 {% if d.AlertNormalValue is defined and d.TeleSignalId is defined %}
                CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", name, name,  pData[kIndex], signal_index_++); 
                {% endif %}
            }
          {% elif d.CValue is defined %}
            {% if d.AlertNormalValue is defined and d.TeleSignalId is defined %}
            //if({{ d.CValue }} != 0xFFFF && {{ d.CValue }} != 0x20) 
            CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}{{ d.TeleSignalName }}{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }}, signal_index_++); 
            {% endif %}
          {% elif d.AlertNormalValue is defined and d.TeleSignalId is defined %}
            //if(pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != 0xFFFF && pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != 0x20) 
            CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}{{ d.TeleSignalName }}{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}], signal_index_++); 
          {% endif %}
	    {% endfor %}
      {% if sc.Cmd == 3 or sc.Cmd == 4 %}
      offset += {{ 2*sc.Len }};
    {% else %}
      offset += {{ sc.Len }};
    {% endif %}
     {% endif %}
	    {% endif %}
  }
{%- endmacro %}
{% macro render_sc(sc, scPrefix=none) -%}
  {
  {% if sc.Data is defined %}
        {% set jsonValueStr = "jsonValue" %}
        {% if sc.TabGroup is defined %}
        {% set jsonValueStr = "jsonValue[\"" + sc.TabGroup + "\"]" %}
        {% endif %}
        {% if sc.Type is defined %}
            {% if sc.Type == 'f' %}
            float pData[{{ (sc.Len*2/4)|int }}];     
            {% elif sc.Type == 'S' %}
            uint16_t pData[{{ sc.Len }}];
            {% elif sc.Type == 's' %}
            int16_t pData[{{ sc.Len }}];
            {% elif sc.Type == "I" %}
            uint32_t pData[{{ (sc.Len*2/4)|int }}];
            {% elif sc.Type == "i" %}
            int32_t pData[{{ (sc.Len*2/4)|int }}];
            {% endif %}
        {% elif sc.Cmd == 3 or sc.Cmd == 4 %}
        uint16_t pData[{{ sc.Len }}];
        {% else %}
        uint8_t pData[{{ sc.Len*2 }}];
        {% endif %}
        memcpy(pData, pCData + offset, {{2*sc.Len}});
	    {% for d in sc.Data %}
      
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          {% if d.Transform is defined and d.Length is defined and d.Transform == "bits" %}
          {
            uint8_t lMemData[8 * {{ d.Length }}];
            int lOffset = 0;
            for(int j=0;j<{{ d.Length }};j++)
            {
                for(int k = 0; k <8; k++)
                {
                    lMemData[j*8+k] = ((pData)[j]>>k)&0x1;
                }
            }
            for(int i={{d.ArrayStart}};i<={{d.ArrayEnd}};i++)
            {
                uint8_t *tMemData = lMemData + lOffset;            
                //_{{ Project.Name|lower }}_{{ d.ArrayBlock }}(rtJ, $tMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $index + $i);
                //$lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
            }
            offset += {{ d.Length }};
          }
          
          {% else %}
          {
            int lOffset = 0;
            for(int i={{d.ArrayStart}};i<={{d.ArrayEnd}};i++)
            {
                std::string tI = boost::lexical_cast<std::string>(i);
                _{{ Project.Name|lower }}_{{ d.ArrayBlock }}((char*)pData + lOffset, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, i, tI.c_str());
                lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
            }
          }
          {% endif %}
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}((char*)pData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, 0, "{{ d.index }}" {% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %} );
      {% else %}
	    {% if d.CValue is defined %}
            {% if d.AlertNormalValue is defined %}
            {{ jsonValueStr }}["{{ d.Name }}"] = ({{ d.CValue }} == {{ d.AlertNormalValue }}) ? "正常" : "告警";
            {{ jsonValueStr }}["AlertArray"]["{{ d.Name }}"] = ({{ d.CValue }} != {{ d.AlertNormalValue }});
            {% elif d.Options is defined %}
            switch({{ d.CValue }}){
            {% for item in d.Options %}
            case {{ item.Key }}:
            {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}"{{ d.Name }}"] = "{{ item.Value }}";
                break;
            {% endfor %}
            default:
                {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}"{{ d.Name }}"] = "无效值";
                break;
            }
            {% else %}
            {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}"{{ d.Name }}"] = ({{ d.CValue }});
            {% endif %}
        {% elif d.ArrayName is defined %}      
            {% if d.Transform is defined and d.ArrayLength is defined and d.Transform == "bits" %}
                {
                    uint8_t lMemData[{{ d.ArrayLength }}];
                    int lOffset = 0;
                    for(int j=0;j<{{ d.ArrayLength }};j++)
                    {

                        lMemData[j] = ((pData)[{{d.Offset}}]>>j)&0x1;
                    }
                    for(int i=1;i<={{ d.ArrayLength }};i++){
                        char nameBuffer[48] = {0};
                        snprintf(nameBuffer, 48, "{{ d.ArrayName }}", {{ d.ArrayStart }} + i);
                        std::string name = nameBuffer;
                        {% if d.Options is defined %}
                        switch(lMemData[i-1]){
                        {% for item in d.Options %}
                        case {{ item.Key }}:
                        {{ jsonValueStr }}[name] = "{{ item.Value }}";
                            break;
                        {% endfor %}
                        default:
                            {{ jsonValueStr }}[name] = "无效值";
                            break;
                        }
                        {% else %}
                        {{ jsonValueStr }}[name] = lMemData[i-1];
                        {% endif %}
                    }
                }
            {% else %}
                for(int i=1;i<={{ d.ArrayLength }};i++){
                    char nameBuffer[48] = {0};
                    snprintf(nameBuffer, 48, "{{ d.ArrayName }}", {{ d.ArrayStart }} + i);
                    std::string name = nameBuffer;
                    int kIndex = {{ d.Offset }} + i;
                    {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}name] = ((float)pData[kIndex]){% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %};
                }
            {% endif %}
        {% elif d.Options is defined %}
            switch(pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}]){
            {% for item in d.Options %}
            case {{ item.Key }}:
            {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}"{{ d.Name }}"] = "{{ item.Value }}";
            {% if item.IsAlert is defined and item.IsAlert %}
            {{ jsonValueStr }}["AlertArray"]["{{ d.Name }}"] = 1;
            {% endif %}
                break;
            {% endfor %}
            default:
                {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}"{{ d.Name }}"] = "无效值";
                break;
            }
        {% elif d.AlertNormalValue is defined %}
            {{ jsonValueStr }}["{{ d.Name }}"] = (pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] == {{ d.AlertNormalValue }}) ? "正常" : "告警";
            {{ jsonValueStr }}["AlertArray"]["{{ d.Name }}"] = (pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }});
        
	    {% else %}
        {{ jsonValueStr }}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}"{{ d.Name }}"] = ((float)pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}]){% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %};
	    {% endif %}
      
        {% if d.Alias is defined %}
        {% for newName in d.Alias %}
        {{ jsonValueStr }}["{{ newName }}"] = {{ jsonValueStr }}["{{ d.Name }}"];
        {% endfor %}

      {% endif %}

        {% endif %}

	    {% endfor %}
      {% if sc.Cmd == 3 or sc.Cmd == 4 %}
      offset += {{ 2*sc.Len }};
    {% else %}
      offset += {{ sc.Len }};
    {% endif %}
	    {% endif %}
  }
{%- endmacro %}




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
        default:{//默认电信动环的情况
            uint8_t* pCData = (uint8_t*)&cData;
            int offset = 4;
            {% for tsc in Sample %}
                {% if tsc.CmdGroupStart is defined %}                
                    for(int cgIndex = {{ tsc.CmdGroupStart }}, index = 1; cgIndex < {{ tsc.CmdGroupEnd }}; cgIndex+={{ tsc.CmdGroupStep}}, index++){
                        std::string namePrefix;
                    {% if tsc.CmdGroupPrefix is string %}
                    char nameBuffer[48] = {0};
                    snprintf(nameBuffer, 48, "{{ tsc.CmdGroupPrefix }}", index);
                    namePrefix = nameBuffer;
                    {% else %}
                    Json::Value namePrefixArray;
                    Json::Reader pReader;//解析
                    if(!pReader.parse("{{ tsc.CmdGroupPrefix }}", namePrefixArray)) {
                    continue;
                    }
                    namePrefix = namePrefixArray[index-1].asString();
                    {% endif %}
                    {% for sc in tsc.CmdGroupSample %}
                    {{ render_sc_alert(sc, "namePrefix") }}
                    {% endfor %}
                    }
                {% else %}
                {{ render_sc_alert(tsc, none) }}
                {% endif %}
            {% endfor %}
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
                    {% if tsc.CmdGroupStart is defined %}                
                        for(int cgIndex = {{ tsc.CmdGroupStart }}, index = 1; cgIndex < {{ tsc.CmdGroupEnd }}; cgIndex+={{ tsc.CmdGroupStep}}, index++){
                            std::string namePrefix;
                        {% if tsc.CmdGroupPrefix is string %}
                        char nameBuffer[48] = {0};
                        snprintf(nameBuffer, 48, "{{ tsc.CmdGroupPrefix }}", index);
                        namePrefix = nameBuffer;
                        {% else %}
                        Json::Value namePrefixArray;
                        Json::Reader pReader;//解析
                        if(!pReader.parse("{{ tsc.CmdGroupPrefix }}", namePrefixArray)) {
                        continue;
                        }
                        namePrefix = namePrefixArray[index-1].asString();
                        {% endif %}
                        {% for sc in tsc.CmdGroupSample %}
                        {{ render_sc(sc, "namePrefix") }}
                        {% endfor %}
                        }
                    {% else %}
                    {{ render_sc(tsc, none) }}
                    {% endif %}
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
