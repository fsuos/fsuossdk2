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

function Get_{{ Project.Name|lower }}_RtData($memData, &$dataArray, $extraPara = false)
{
    if (strlen($memData) == false) {
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    } else {
        $dataArray['无数据'] = false;
        $offset = 4;
        {% for sc in Sample %}
        {% if sc.Cmd == 3 or sc.Cmd == 4 or sc.Cmd == 5 %}
        $v = unpack('C*', substr($memData, $offset, {{ sc.Offset|length }}));
        {% for d in sc.Data %}
        {% if d.Name is defined %}           
            {% if d.AlertNormalValue is defined %}
            _{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ d.Name }}", $v[{{ loop.index }}], {{ d.AlertNormalValue }});       
            {% elif d.AlertAbnormalValue is defined %}
            _{{ Project.Name|lower }}_ShowNormal($dataArray, "{{ d.Name }}", $v[{{ loop.index }}], {{ d.AlertAbnormalValue }});       
            {% else %}
             $dataArray["{{ d.Name }}"] = $v[{{ loop.index }}]{% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
            {% endif %}
        {% else %}
            $dataArray["{{ d }}"] = $v[{{ loop.index }}];
        {% endif %}
        {% endfor %}
        $offset += {{ sc.Offset|length }};
        {% elif sc.Cmd == 0 or sc.Cmd == 1 or sc.Cmd == 2 %}
        $v = unpack('f*', substr($memData, $offset, 4*{{ sc.Offset|length }}));
        {% for d in sc.Data %}
        {% if d.Name is defined %}
            {% if d.AlertNormalValue is defined %}
            _{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ d.Name }}", $v[{{ loop.index }}], {{ d.AlertNormalValue }});       
            {% elif d.AlertAbnormalValue is defined %}
            _{{ Project.Name|lower }}_ShowNormal($dataArray, "{{ d.Name }}", $v[{{ loop.index }}], {{ d.AlertAbnormalValue }});       
            {% else %}
             $dataArray["{{ d.Name }}"] = number_format($v[{{ loop.index }}], 3){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
            {% endif %}
        {% else %}
            $dataArray["{{ d }}"] = number_format($v[{{ loop.index }}], 3);
        {% endif %}
        {% endfor %}
        $offset += 4*{{ sc.Offset|length }};
        {% elif sc.Cmd == 19 %}
        $v = unpack('I*', substr($memData, $offset, 4*{{ sc.Offset|length }}));
        {% for d in sc.Data %}
        {% if d.Name is defined %}
            {% if d.AlertNormalValue is defined %}
            _{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ d.Name }}", $v[{{ loop.index }}], {{ d.AlertNormalValue }});       
            {% elif d.AlertAbnormalValue is defined %}
            _{{ Project.Name|lower }}_ShowNormal($dataArray, "{{ d.Name }}", $v[{{ loop.index }}], {{ d.AlertAbnormalValue }});       
            {% else %}
             $dataArray["{{ d.Name }}"] = $v[{{ loop.index }}]{% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
            {% endif %}
        {% else %}
            $dataArray["{{ d }}"] = $v[{{ loop.index }}];
        {% endif %}
        {% endfor %}
        $offset += 4*{{ sc.Offset|length }};
        {% endif %}
        {% endfor %}
        $v = unpack('v', substr($memData, $offset, 2));
        $year = $v[1];
        $offset += 2;
        $v = unpack('C*', substr($memData, $offset, 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));
    }
}
