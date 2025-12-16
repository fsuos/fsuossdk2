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
            {% for d in tsc.Data %}
              {% if d.Type is not defined or d.Type == 'C'%}
              $len = 1;
              $type = 'C';
              {% elif d.Type == 'S' or d.Type == 's' %}
              $len = 2;
              $type = '{{ d.Type }}';
              {% elif d.Type == 'f' %}
              $len = 4;
              $type = '{{ d.Type }}';
              {% endif %}
              $v = unpack($type , strrev(substr($memData , $offset + {{ d.Offset }}, $len)));
              {% if d.AlertNormalValue is defined %}
                  _{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ d.Name }}", $v[1], {{ d.AlertNormalValue }});
                  {% elif d.AlertAbnormalValue is defined %}
                  _{{ Project.Name|lower }}_ShowNormal($dataArray, "{{ d.Name }}", $v[1], {{ d.AlertAbnormalValue }});
              {% else %}
                  $dataArray["{{ d.Name }}"] = number_format($v[1]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
              {% endif %}
            {% endfor %}
            $offset += {{ tsc.Len }};
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

