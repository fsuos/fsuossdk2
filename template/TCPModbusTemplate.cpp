#include "common_define.h"
#include "{{ Project.Name }}.h"
#include "UniDataDevice.cpp"
#include <future>

{% if BlockTemplate is defined %}
{% for key,blockDef in BlockTemplate.items() %}
int {{ Project.Name }}::_{{ Project.Name|lower }}_{{ key }}_alert(char* pCData,const char* prefix, int iIndex, const char* index {% if blockDef.HasIndex1 is defined  %}, const char* index1{% endif %} {% if blockDef.HasIndex2 is defined %}, const char* index2{% endif %})
{
          int offset = 0;
    {% if blockDef.BlockRType is defined %}
        {% if blockDef.BlockRType == 'f' %}
        float pData[{{ (blockDef.BlockLength/4)|int }}];
        memcpy(pData, pCData, {{blockDef.BlockLength}}); 
        {% elif blockDef.BlockRType == 'S' %}
        uint16_t pData[{{ (blockDef.BlockLength/2)|int }}];
        memcpy(pData, pCData, {{blockDef.BlockLength}});
        for(int i=0; i< {{ (blockDef.BlockLength/2)|int }}; i++){
            pData[i] = ntohs(pData[i]);
        }
        memcpy(pData, pCData, {{blockDef.BlockLength}});
        {% elif blockDef.BlockRType == 's' %}
        int16_t pData[{{ sc.Len }}];
        memcpy(pData, pCData, {{blockDef.BlockLength}});
        for(int i=0; i< {{ (blockDef.BlockLength/2)|int }}; i++){
            pData[i] = ntohs(pData[i]);
        }
        memcpy(pData, pCData, {{blockDef.BlockLength}});
        {% elif blockDef.BlockRType == "I" %}
        uint32_t pData[{{ (sc.Len*2/4)|int }}];
        memcpy(pData, pCData, {{blockDef.BlockLength}});
        {% elif blockDef.BlockRType == "i" %}
        int32_t pData[{{ (sc.Len*2/4)|int }}];
        memcpy(pData, pCData, {{blockDef.BlockLength}});
        {% endif %}
    {% elif blockDef.BlockType is defined %}
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
      _{{ Project.Name|lower }}_{{ d.Block }}_alert(pData + offset , prefix, iIndex, {% if d.index is defined %}"{{ d.index }}"{% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %}{% else %}index{% endif %});
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
          {% if d.AlertNormalValue is defined or d.AlertAbnormalValue is defined %}            
            {% if  d.TeleSignalId is defined %}
            if(b_mode_ == 2){
                
                {% if d.TeleSignalId|string|length == 12 %}
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name,  ({{ d.CValue }}) != {{ d.AlertNormalValue }}, signal_index_++); 
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name,  ({{ d.CValue }}) == {{ d.AlertAbnormalValue }}, signal_index_++); 
                    {% endif %}
                {% else %}
                {
                    std::stringstream ss;
                    ss<<"{{ d.TeleSignalId }}" << std::setw(3)<<std::setfill('0')<<signal_index_<<"0";
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, ss.str(), ss.str(), {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name, ({{ d.CValue }}) != {{ d.AlertNormalValue }}, signal_index_++); 
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, ss.str(), ss.str(), {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name, ({{ d.CValue }}) == {{ d.AlertAbnormalValue }}, signal_index_++); 
                    {% endif %}
                }
                {% endif %}
            }
            {% endif %}
            {% if d.UnicomSignalId is defined %}
            if(b_mode_ == 1){
            {% if d.AlertNormalValue is defined %}
            CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}name{% endif %}, name,  ({{ d.CValue }}) != {{ d.AlertNormalValue }}, signal_index_++); 
            {% elif d.AlertAbnormalValue is defined %}
            CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}name{% endif %}, name,  ({{ d.CValue }}) == {{ d.AlertAbnormalValue }}, signal_index_++); 
            {% endif %}
            }
            {% endif %}
          {% endif %}
        {% elif d.AlertNormalValue is defined or d.AlertAbnormalValue is defined %}
          {% if d.TeleSignalId is defined %}
            if(b_mode_ == 2){
                 {% if d.TeleSignalId|string|length == 12 %}
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name,  pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }}, signal_index_++); 
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name,  pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] == {{ d.AlertAbnormalValue }}, signal_index_++); 
                    {% endif %}
                 {% else %}
                 {
                    std::stringstream ss;
                    ss<<"{{ d.TeleSignalId }}" << std::setw(3)<<std::setfill('0')<<signal_index_<<"0";
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, ss.str(), ss.str(), name, name,  pData[kIndex] != {{ d.AlertNormalValue }}, signal_index_++);
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, ss.str(), ss.str(), name, name,  pData[kIndex] == {{ d.AlertAbnormalValue }}, signal_index_++);
                    {% endif %}
                 }
                 {% endif %}
            }
            {% endif %}
            {% if d.UnicomSignalId is defined %}
                 if(b_mode_ == 1){
                {% if d.AlertNormalValue is defined %}
                 CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name,  pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }}, signal_index_++); 
                {% elif d.AlertAbnormalValue is defined %}
                 CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name,  pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertAbnormalValue }}, signal_index_++); 
                 {% endif %}
                 }
            {% endif %}
        {% elif d.ArrayName is defined %}
        for(int i=0;i<{{ d.ArrayLength }};i++){
                char nameBuffer[48] = {0};
                snprintf(nameBuffer, 48, "{{ d.ArrayName }}", {{ d.ArrayStart }} + i);
                std::string name = nameBuffer;
                int kIndex = {{ d.Offset }} + i;
                 {% if d.AlertNormalValue is defined or d.AlertAbnormalValue is defined %}
                 {% if d.TeleSignalId is defined %}
                 if(b_mode_ == 2){
                 {% if d.TeleSignalId|string|length == 12 %}
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name, pData[kIndex] != {{ d.AlertNormalValue }}, signal_index_++); 
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}name{% endif %}, name, pData[kIndex] == {{ d.AlertAbnormalValue }}, signal_index_++); 
                    {% endif %}
                 {% else %}
                 {
                    std::stringstream ss;
                    ss<<"{{ d.TeleSignalId }}" << std::setw(3)<<std::setfill('0')<<signal_index_<<"0";
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, ss.str(), ss.str(), name, name,  pData[kIndex] != {{ d.AlertNormalValue }}, signal_index_++);
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, ss.str(), ss.str(), name, name,  pData[kIndex] == {{ d.AlertAbnormalValue }}, signal_index_++);
                    {% endif %}
                 }
                 {% endif %}
                 }
                {% endif %}
                 {% if d.UnicomSignalId is defined %}
                 if(b_mode_ == 1){
                    {% if d.AlertNormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}name{% endif %}, name,  pData[kIndex] != {{ d.AlertNormalValue }}, signal_index_++); 
                    {% elif d.AlertAbnormalValue is defined %}
                    CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}name{% endif %}, name,  pData[kIndex] == {{ d.AlertAbnormalValue }}, signal_index_++); 
                    {% endif %}
                 }
                 {% endif %}
                {% endif %}
            }
         {% endif %}
        }
      {% endif %}
    {% endfor %}
    return {{blockDef.BlockLength}};
}
{% endfor %}
{% endif %}

{% macro render_sc_alert(sc, vendor, scPrefix=none) -%}
  {
  {% if sc.Data is defined %}
        {% set hasAlert = namespace(found=true) %}
        {% for d in sc.Data %}
           {% if d.AlertNormalValue is defined and ( d.TelePSignalId is defined or d.TeleSignalId is defined or d.UnicomSignalId is defined ) %}
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
            memcpy(pData, pCData + offset, {{2*sc.Len}});
            {% elif sc.Type == 's' %}
            int16_t pData[{{ sc.Len }}];
            memcpy(pData, pCData + offset, {{2*sc.Len}});
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
        int lOffset = 0, usedLen = 0;
	    {% for d in sc.Data %}
          {% if d.Block is defined %}
          usedLen = _{{ Project.Name|lower }}_{{ d.Block }}_alert((char*)pData + lOffset, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, 0, "{{ d.index }}" {% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %});
          lOffset += usedLen;
          {% elif d.ArrayName is defined %}
          for(int i=0;i<{{ d.ArrayLength }};i++){
                char nameBuffer[48] = {0};
                snprintf(nameBuffer, 48, "{{ d.ArrayName }}", {{ d.ArrayStart }} + i);
                std::string name = nameBuffer;
                int kIndex = {{ d.Offset }} + i;
                 {% if d.AlertNormalValue is defined  %}
                 {% if vendor == 2 and d.TeleSignalId is defined %}
                 CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", name, name,  pData[kIndex], signal_index_++); 
                 {% elif vendor == 1 and d.UnicomSignalId is defined %}
                 CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", name, name,  pData[kIndex], signal_index_++); 
                 {% elif vendor == 0 and d.TelePSignalId is defined %}
                 CheckThresholdBool(2, "{{ d.TelePSignalId }}", "{{ d.TelePSignalId }}", name, name,  pData[kIndex], signal_index_++); 
                 {% endif %}
                {% endif %}
            }
          {% elif d.CValue is defined %}
            {% if d.AlertNormalValue is defined %}
            //if({{ d.CValue }} != 0xFFFF && {{ d.CValue }} != 0x20) 
                {% if vendor == 2 and d.TeleSignalId is defined %}
                CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }} != {{ d.AlertNormalValue }}, signal_index_++); 
                {% elif vendor == 1 and d.UnicomSignalId is defined %}
                CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }} != {{ d.AlertNormalValue }}, signal_index_++); 
                {% elif vendor == 0 and d.TelePSignalId is defined %}
                CheckThresholdBool(2, "{{ d.TelePSignalId }}", "{{ d.TelePSignalId }}", {% if d.TelePSignalName is defined %}"{{ d.TelePSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }} != {{ d.AlertNormalValue }}, signal_index_++); 
                {% endif %}
            {% elif d.AlertAbnormalValue is defined %}
                {% if vendor == 2 and d.TeleSignalId is defined %}
                CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }} == {{ d.AlertAbnormalValue }}, signal_index_++); 
                {% elif vendor == 1 and d.UnicomSignalId is defined %}
                CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }} == {{ d.AlertAbnormalValue }}, signal_index_++); 
                {% elif vendor == 0 and d.TelePSignalId is defined %}
                CheckThresholdBool(2, "{{ d.TelePSignalId }}", "{{ d.TelePSignalId }}", {% if d.TelePSignalName is defined %}"{{ d.TelePSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  {{ d.CValue }} == {{ d.AlertAbnormalValue }}, signal_index_++); 
                {% endif %}
            {% endif %}
          {% elif d.AlertNormalValue is defined %}
            //if(pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != 0xFFFF && pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != 0x20) 
            {% if vendor == 2 and  d.TeleSignalId is defined %}
            CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }}, signal_index_++); 
            {% elif vendor == 1 and d.UnicomSignalId is defined %}
            CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }}, signal_index_++); 
            {% elif vendor == 0 and d.TelePSignalId is defined %}
            CheckThresholdBool(2, "{{ d.TelePSignalId }}", "{{ d.TelePSignalId }}", {% if d.TelePSignalName is defined %}"{{ d.TelePSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] != {{ d.AlertNormalValue }}, signal_index_++); 
            {% endif %}
          {% elif d.AlertAbnormalValue is defined %}
            {% if vendor == 2 and  d.TeleSignalId is defined %}
            CheckThresholdBool(2, "{{ d.TeleSignalId }}", "{{ d.TeleSignalId }}", {% if d.TeleSignalName is defined %}"{{ d.TeleSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] == {{ d.AlertAbnormalValue }}, signal_index_++); 
            {% elif vendor == 1 and d.UnicomSignalId is defined %}
            CheckThresholdBool(2, "{{ d.UnicomSignalId }}", "{{ d.UnicomSignalId }}", {% if d.UnicomSignalName is defined %}"{{ d.UnicomSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}", pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] == {{ d.AlertAbnormalValue }}, signal_index_++); 
            {% elif vendor == 0 and d.TelePSignalId is defined %}
            CheckThresholdBool(2, "{{ d.TelePSignalId }}", "{{ d.TelePSignalId }}", {% if d.TelePSignalName is defined %}"{{ d.TelePSignalName }}"{%else%}"{{ d.Name }}"{% endif %}, "{{ d.Name }}",  pData[{% if d.Offset is defined %}{{ d.Offset-1 }}{% else %}{{ loop.index-1 }}{% endif %}] == {{ d.AlertAbnormalValue }}, signal_index_++); 
            {% endif %}
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
                    {{ render_sc_alert(sc, 1, "namePrefix") }}
                    {% endfor %}
                    }
                {% else %}
                {{ render_sc_alert(tsc, 1, none) }}
                {% endif %}
            {% endfor %}
            break;
        }
        case 2://电信
        {
             {% if RunCheckThresholdCodeTelecom is defined %}{{ RunCheckThresholdCodeTelecom }}{% endif %}
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
                    {{ render_sc_alert(sc, 2, "namePrefix") }}
                    {% endfor %}
                    }
                {% else %}
                {{ render_sc_alert(tsc, 2, none) }}
                {% endif %}
            {% endfor %}
            break;
        }
        case 3://移动
        {
            {% if RunCheckThresholdCodeMobile is defined %}{{ RunCheckThresholdCodeMobile }}{% endif %}
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
                    {{ render_sc_alert(sc, 3, "namePrefix") }}
                    {% endfor %}
                    }
                {% else %}
                {{ render_sc_alert(tsc, 3, none) }}
                {% endif %}
            {% endfor %}
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
                    {{ render_sc_alert(sc, 0, "namePrefix") }}
                    {% endfor %}
                    }
                {% else %}
                {{ render_sc_alert(tsc, 0, none) }}
                {% endif %}
            {% endfor %}
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
            //std::async( std::launch::async,
            //            [&]() {
                int nRet = modbus_connect(pCtx);
                if(-1 == nRet)
                {
                    //printf("connect failed\n");
        	    state = 0;
                    modbus_close(pCtx);
                    modbus_free(pCtx);
                    pCtx = nullptr;
                    return false;
                }
                modbus_set_slave(pCtx, addr_);
                uint16_t regs[125];
                uint8_t bits[2000];
                {% for tsc in Sample %}
                    {% if tsc.CmdGroupStart is defined %}  
                        for(int cmdgroup_step_ = 0; cmdgroup_step_ < {{ "%d"%((tsc.CmdGroupEnd-tsc.CmdGroupStart)/tsc.CmdGroupStep) }}; cmdgroup_step_++){             
                        {% for sc in tsc.CmdGroupSample %}                        
                        {% if sc.Cmd == 3 %}
                        //modbus_read_registers({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});                        
                        if(-1 == modbus_read_registers(pCtx, {{ tsc.CmdGroupStart + sc.Offset }} + {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ sc.Len }}, regs)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	    	    state = 0;
                            return false;
                        }
                        memcpy(cData.{{ "r_%d[cmdgroup_step_]"%(tsc.CmdGroupStart ) }}.{{ "r%d_%d_%d"%(sc.Cmd,tsc.CmdGroupStart,sc.Offset ) }}, regs, sizeof(uint16_t)*{{ sc.Len }});
                        {% elif sc.Cmd == 4 %}
                        //modbus_read_input_registers({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                        if(-1 == modbus_read_input_registers(pCtx, {{ tsc.CmdGroupStart + sc.Offset }} + {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ sc.Len }}, regs)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.{{ "r_%d[cmdgroup_step_]"%(tsc.CmdGroupStart ) }}.{{ "r%d_%d_%d"%(sc.Cmd,tsc.CmdGroupStart,sc.Offset ) }}, regs, sizeof(uint16_t)*{{ sc.Len }});
                        {% elif sc.Cmd == 1 %}
                        //modbus_read_bits({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                        if(-1 == modbus_read_bits(pCtx, {{ tsc.CmdGroupStart + sc.Offset }} + {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ sc.Len }}, bits)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.{{ "r_%d[cmdgroup_step_]"%(tsc.CmdGroupStart ) }}.{{ "b%d_%d_%d"%(sc.Cmd,tsc.CmdGroupStart,sc.Offset ) }}, bits, {{ sc.Len }});
                        {% elif sc.Cmd == 2 %}
                        //modbus_read_input_bits({{ tsc.CmdGroupStart + sc.Offset }}, {{ sc.Len }});
                        if(-1 == modbus_read_input_bits(pCtx, {{ tsc.CmdGroupStart + sc.Offset }} + {{ tsc.CmdGroupStep}}*cmdgroup_step_, {{ sc.Len }}, bits)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.{{ "r_%d[cmdgroup_step_]"%(tsc.CmdGroupStart ) }}.{{ "b%d_%d_%d"%(sc.Cmd,tsc.CmdGroupStart,sc.Offset ) }}, bits, {{ sc.Len }});
                        {% endif %}
                        {% endfor %}
                        }
                    {% else %}
                        {% if tsc.Cmd == 3 %}
                        if(-1 == modbus_read_registers(pCtx, {{ tsc.Offset }}, {{ tsc.Len }}, regs)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.r3_{{ tsc.Offset }}, regs, sizeof(uint16_t)*{{ tsc.Len }});
                        {% elif tsc.Cmd == 4 %}
                        if(-1 == modbus_read_input_registers(pCtx, {{ tsc.Offset }}, {{ tsc.Len }}, regs)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.r4_{{ tsc.Offset }}, regs, sizeof(uint16_t)*{{ tsc.Len }});
                        {% elif tsc.Cmd == 1 %}
                        if(-1 == modbus_read_bits(pCtx, {{ tsc.Offset }}, {{ tsc.Len }}, bits)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.b1_{{ tsc.Offset }}, bits, {{ tsc.Len }});
                        {% elif tsc.Cmd == 2 %}
                        if(-1 == modbus_read_input_bits(pCtx, {{ tsc.Offset }}, {{ tsc.Len }}, bits)){
                            modbus_close(pCtx);
                            modbus_free(pCtx);
                            pCtx = nullptr;
        	            state = 0;
                            return false;
                        }
                        memcpy(cData.b2_{{ tsc.Offset }}, bits, {{ tsc.Len }});
                        {% endif %}
                    {% endif %}                  
                {% endfor %}
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
                RoundDone();
        	state = 0;
                return false;
            //});
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
