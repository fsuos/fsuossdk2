{% macro render_sc(sc, scPrefix=none) -%}
  {% if sc.Data is defined %}
        {% set dataArrayStr = "$dataArray" %}
        {% if sc.TabGroup is defined %}
        {% set dataArrayStr = "$dataArray[\"" + sc.TabGroup + "\"]" %}
        {% endif %}
        {% if sc.RType is defined %}
        $tMemData = substr($memData , $offset, 2*{{ sc.Len }});
        $lMemData = '';
        {% if sc.RType == "f" or sc.RType == "I" or sc.RType == "i" %}
        for($i=0;$i<strlen($tMemData);$i+=4){
          $lMemData .= $tMemData[$i+2].$tMemData[$i+3].$tMemData[$i].$tMemData[$i+1];
        }
        {% else %}
        for($i=0;$i<strlen($tMemData);$i+=2){
          $lMemData .= $tMemData[$i+1].$tMemData[$i];
        }
        {% endif %}
        $v = unpack("{{sc.RType}}*" , $lMemData);
        {% elif sc.Type is defined %}        
$v = unpack("{{sc.Type}}*" , substr($memData , $offset, 2*{{ sc.Len }}));
$lMemData = substr($memData , $offset, 2*{{ sc.Len }});
        {% elif sc.Cmd == 3 or sc.Cmd == 4 %}
$v = unpack("S*" , substr($memData , $offset, 2*{{ sc.Len }}));
$lMemData = substr($memData , $offset, 2*{{ sc.Len }});
        {% else %}
$v = unpack("C*" , substr($memData , $offset, {{ sc.Len }}));
$lMemData = substr($memData , $offset, {{ sc.Len }});
        {% endif %}
        {% if sc.Transform is defined and sc.Transform == "bits" %}
          $lMemData = '';
          for($j=1;$j<=count($v);$j++)
          {
              for($k = 0; $k <8; $k++)
              {
                $lMemData .= pack("C", ($v[$j]>>$k)&0x1);
              }
          }
        {% endif %}
      $lOffset = 0;
	    {% for d in sc.Data %}      
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          {% if d.Transform is defined and d.Length is defined and d.Transform == "bits" %}
          $lMemData = substr($memData, $offset, {{ d.Length }});
          $v = unpack('C*', $lMemData);
          $lMemData = '';
          for($j=1;$j<=count($v);$j++)
          {
              for($k = 0; $k <8; $k++)
              {
                $lMemData .= pack("C", ($v[$j]>>$k)&0x1);
              }
          }
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($lMemData, $offset + $lOffset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}({{ dataArrayStr }}, $tMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $i);
            $lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          $offset += {{ d.Length }};
          {% else %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($lMemData, $lOffset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}({{ dataArrayStr }}, $tMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $i);
            $lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          {% endif %}
        {% endif %}
      {% elif d.Block is defined %}
      $tMemData = substr($lMemData, $lOffset, {{ BlockTemplate[d.Block]["BlockLength"] }});
      _{{ Project.Name|lower }}_{{ d.Block }}({{ dataArrayStr }}, $tMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, {% if d.index is defined %} "{{ d.index }}" {% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %} {% else %}$index{% endif %});
      $lOffset += {{ BlockTemplate[d.Block]["BlockLength"] }};
      {% else %}
	    {% if d.Value is defined %}
         {% if d.Options is defined %}
        switch({{ d.Value }}){
        {% for item in d.Options %}
          case {{ item.Key }}:
          {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}"] = "{{ item.Value }}";
            break;
        {% endfor %}
          default:
            {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}"] = "无效值";
            break;
        }
        {% else %}
        {% if d.AlertNormalValue is defined %}
        _{{ Project.Name|lower }}_ShowAlert({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}", {{ d.Value }}, {{ d.AlertNormalValue }});       
        {% elif d.AlertAbnormalValue is defined %}
        _{{ Project.Name|lower }}_ShowNormal({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}", {{ d.Value }}, {{ d.AlertAbnormalValue }});       
        {% else %}
        {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}"] = ({{ d.Value }}){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
        {% endif %}
        {% endif %}
      {% elif d.ArrayName is defined %}
            {% if d.Transform is defined and d.ArrayLength is defined and d.Transform == "bits" %}
                $lMemData = [];
                for($j=0;$j<{{ d.ArrayLength }};$j++)
                {
                   $lMemData[$j]= $v[{{ d.Offset }}]>>$j;
                }
                for($i=1;$i<={{ d.ArrayLength }};$i++){
                    $name = sprintf("{{ d.ArrayName }}", {{ d.ArrayStart }} + $i);
                    {% if d.Options is defined %}
                    switch($lMemData[$i-1]){
                    {% for item in d.Options %}
                    case {{ item.Key }}:
                      {{ dataArrayStr }}[$name]  = "{{ item.Value }}";
                        break;
                    {% endfor %}
                    default:
                    {{ dataArrayStr }}[$name]  = "无效值";
                        break;
                    }
                    {% else %}
                    {{ dataArrayStr }}[$name] =  = $lMemData[i-1];
                    {% endif %}
                }
            {% else %}
          for($i=1;$i<={{ d.ArrayLength }};$i++){
            $name = sprintf("{{ d.ArrayName }}", {{ d.ArrayStart }} + $i);
            $kIndex = {% if d.Offset is defined %}{{ d.Offset }}{% else %}0{% endif %} + $i;
            {% if d.AlertNormalValue is defined %}
            _{{ Project.Name|lower }}_ShowAlert({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"$name", $v[$kIndex]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, {{ d.AlertNormalValue }});  
            {% elif d.AlertAbnormalValue is defined %}
            _{{ Project.Name|lower }}_ShowNormal({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"$name", $v[$kIndex]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, {{ d.AlertAbnormalValue }});  
            {% else %}
            {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}$name] = number_format($v[$kIndex]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, 3){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
            {% endif %}
          }
          {% endif %}
    {% elif d.AlertNormalValue is defined %}
        _{{ Project.Name|lower }}_ShowAlert({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}", $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, {{ d.AlertNormalValue }});       
    {% elif d.AlertAbnormalValue is defined %}
        _{{ Project.Name|lower }}_ShowNormal({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}", $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, {{ d.AlertAbnormalValue }});       
    {% elif d.Options is defined %}
        switch($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]){
        {% for item in d.Options %}
          case {{ item.Key }}:
          {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}"] = "{{ item.Value }}";
          {% if item.IsAlert is defined and item.IsAlert %}
          {{ dataArrayStr }}["AlertArray"]["{{ d.Name }}"] = 1;
          {% endif %}
            break;
        {% endfor %}
          default:
            {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}"] = "无效值";
            break;
        }
        {% elif d.AlertNormalValue is defined %}
        _{{ Project.Name|lower }}_ShowAlert({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}", $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], {{ d.AlertNormalValue }});
        {% elif d.AlertAbnormalValue is defined %}
        _{{ Project.Name|lower }}_ShowNormal({{ dataArrayStr }}, {% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}", $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], {{ d.AlertAbnormalValue }});
	   {% else %}
        {{ dataArrayStr }}[{% if scPrefix is not none %}{{scPrefix}}.{% endif %}"{{ d.Name }}"] = number_format($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
	    {% endif %}
      
        {% if d.Alias is defined %}
        {% for newName in d.Alias %}
        {{ dataArrayStr }}["{{ newName }}"] = {{ dataArrayStr }}["{{ d.Name }}"];
        {% endfor %}

      {% endif %}
        {% endif %}

	    {% endfor %}
      {% if sc.Cmd == 3 or sc.Cmd == 4 %}
      $offset += {{ 2*sc.Len }};
    {% else %}
      $offset += {{ sc.Len }};
    {% endif %}
	    {% endif %}
{%- endmacro %}
<?php
function _{{ Project.Name|lower }}_ShowNormal(&$dataArray, $label, $value, $vsValue)
{
    // if($value == 0xFFFF || $value == 0x20){
    //   $dataArray[$label] = "无效值";
    // }else{
      $dataArray[$label] = $value == $vsValue ? '告警' : '正常';
      $dataArray['AlertArray'][$label] = $value == $vsValue;
    //}
}

function _{{ Project.Name|lower }}_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    // if($value == 0xFFFF || $value == 0x20){
    //   $dataArray[$label] = "无效值";
    // }else{
      $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
      $dataArray['AlertArray'][$label] = $value != $vsValue;
    //}
}

{% if BlockTemplate is defined %}
{% for key,blockDef in BlockTemplate.items() %}
function _{{ Project.Name|lower }}_{{ key }}(&$dataArray, $memData, $prefix, $index {% if blockDef.HasIndex1 is defined  %},$index1{% endif %} {% if blockDef.HasIndex2 is defined %},$index2{% endif %})
{
    $offset = 0;
    {% if blockDef.BlockRType is defined %}
      {% if blockDef.BlockRType == 'f' or blockDef.BlockRType == 'I' or blockDef.BlockRType == 'i' %}
      $lMemData = '';
      for($i=0;$i<strlen($memData);$i+=4){
        $lMemData .= $memData[$i+2].$memData[$i+3].$memData[$i].$memData[$i+1];
      }
      $v = unpack("{{blockDef.BlockRType}}*" , $lMemData);
      {% else %}
      $lMemData = '';
      for($i=0;$i<strlen($memData);$i+=2){
        $lMemData .= $memData[$i+1].$memData[$i];
      }
      $v = unpack("{{blockDef.BlockRType}}*" , $lMemData);
      {% endif %}
    {% elif blockDef.BlockType is defined %}
    $v = unpack("{{blockDef.BlockType}}*" , $memData);
    {% else %}
    $v = unpack("S*" , $memData);
    {% endif %}
    {% for d in blockDef.BlockContent %}
      
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          //这个地方
          {% if d.Transform is defined and d.Length is defined and d.Transform == "bits" %}
          $lMemData = substr($memData, $offset, {{ d.Length }});
          $lOffset = 0;
          $v = unpack('C*', $lMemData);
          $lMemData = '';
          for($j=1;$j<=count($v);$j++)
          {
              for($k = 0; $k <8; $k++)
              {
                $lMemData .= pack("C", ($v[$j]>>$k)&0x1);
              }
          }
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($lMemData, $offset + $lOffset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($dataArray, $tMemData, $prefix, $index + $i);
            $lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          {% else %}
          $lOffset = 0;
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($memData,  $offset + $lOffset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($dataArray, $tMemData, $prefix, $index + $i);
            $lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          $offset += {{ d.Length }};
          {% endif %}          
        {% endif %}
      {% elif d.Block is defined %}
      $lMemData = substr($memData, $offset, {{ BlockTemplate[d.Block]["BlockLength"] }});
      _{{ Project.Name|lower }}_{{ d.Block }}($dataArray, $lMemData, $prefix, {% if d.index is defined %}"{{ d.index }}"{% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %}{% else %}$index{% endif %});
      $offset += {{ BlockTemplate[d.Block]["BlockLength"] }};
      {% else %}
        $name = $prefix.sprintf("{{ d.Name }}", {% if d.Index is defined %}{{ d.Index }}{% else %}strval($index){% endif %});
        {% if d.Value is defined %}
          {% if d.AlertNormalValue is defined %}
          _{{ Project.Name|lower }}_ShowAlert($dataArray, $name, {{ d.Value }}, {{ d.AlertNormalValue }});
          {% elif d.AlertAbnormalValue is defined %}
          _{{ Project.Name|lower }}_ShowNormal($dataArray, $name, {{ d.Value }}, {{ d.AlertAbnormalValue }});
          {% elif d.Options is defined %}
          
          switch({{ d.Value }}){
          {% for item in d.Options %}
            case {{ item.Key }}:
            $dataArray[$name] = "{{ item.Value }}";
              break;
          {% endfor %}
            default:
            $dataArray[$name]  = "无效值";
              break;
          }
          {% else %}
          $dataArray[$name]  = {{ d.Value }}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %};
          {% endif %}
          {% elif d.Options is defined %}
          switch($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]){
          {% for item in d.Options %}
            case {{ item.Key }}:
            $dataArray[$name] = "{{ item.Value }}";
              break;
          {% endfor %}
            default:
              $dataArray[$name] = "无效值";
              break;
          }
          {% elif d.ArrayName is not defined and d.AlertNormalValue is defined %}
          _{{ Project.Name|lower }}_ShowAlert($dataArray, $name, $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], {{ d.AlertNormalValue }});
          {% elif d.ArrayName is not defined and d.AlertAbnormalValue is defined %}
          _{{ Project.Name|lower }}_ShowNormal($dataArray, $name, $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], {{ d.AlertAbnormalValue }});
       {% elif d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          $name = $prefix.sprintf("{{ d.ArrayName }}", {{ d.ArrayStart }} + $i);
          $kIndex = {{ d.Offset }} + $i;
          {% if d.AlertNormalValue is defined %}
          _{{ Project.Name|lower }}_ShowAlert($dataArray, $name, $v[$kIndex], {{ d.AlertNormalValue }});
          {% elif d.AlertAbnormalValue is defined %}
          _{{ Project.Name|lower }}_ShowNormal($dataArray, $name, $v[$kIndex], {{ d.AlertAbnormalValue }});
          {% else %}
          $dataArray[$name] = number_format($v[$kIndex]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
          {% endif %}
        }
        {% else %}
          $dataArray[$name] = number_format($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}{% if d.Adjust is defined %}{{ d.Adjust }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
        {% endif %}
      {% endif %}
          {% if d.Alias is defined %}
          {% for newName in d.Alias %}
          $dataArray["{{ newName }}"] = $dataArray["{{ d.Name }}"];
          {% endfor %}
          {% endif %}
    {% endfor %}
}
{% endfor %}
{% endif %}

{% if InitSetting is defined %}
function Is_{{ Project.Name|lower }}_NeedExtraPara(){
  return true;
}
{% endif %}

function Get_{{ Project.Name|lower }}_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();

        //InitSetting
        {% if InitSetting is defined %}
        {% for s in InitSetting %}
          ${{s.Name}} = $extraPara->{{s.Name}};
          if(!${{s.Name}}){
            ${{s.Name}} = {% if s.Default is defined %}{{s.Default}}{% else %}0{% endif %};
          }          
        {% endfor %}
        {% endif %}


        $offset = 4;
        {% for tsc in Sample %}
            {% if tsc.CmdGroupStart is defined %}
            
            for($cgIndex = {{ tsc.CmdGroupStart }},$index = 1; $cgIndex < {{ tsc.CmdGroupEnd }}; $cgIndex+={{ tsc.CmdGroupStep}}, $index++){
              {% if tsc.CmdGroupPrefix is string %}
              $namePrefix = sprintf("{{ tsc.CmdGroupPrefix }}", $index);
              {% else %}
              $namePrefixArray = {{ tsc.CmdGroupPrefix }};
              $namePrefix = $namePrefixArray[$index-1];
              {% endif %}
            {% for sc in tsc.CmdGroupSample %}
            {{ render_sc(sc, "$namePrefix") }}
            {% endfor %}
            }
            {% else %}
            {{ render_sc(tsc, none) }}
            {% endif %}
        {% endfor %}
        $v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));
      {% if ExtraPhpCode is defined %}
      {{ ExtraPhpCode }}
      {% endif %}
    }
}

