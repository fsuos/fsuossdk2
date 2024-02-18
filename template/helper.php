<?php
function _{{ Project.Name|lower }}_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_{{ Project.Name|lower }}_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
	{% for sc in Sample %}
            {% if sc.Cmd == 3 or sc.Cmd == 4 %}
	    {% if sc.Type is defined %}
        {% if sc.Type == "f" or sc.Type == "I" or sc.Type == "i" %}
 	    $mem = substr($memData, $offset, 2*{{ sc.Len }});
        $newMem = '';
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("{{sc.Type}}*" , $newMem);
        {% else %}
        $v = unpack("{{sc.Type}}*" , substr($memData , $offset, 2*{{ sc.Len }}));
        {% endif %}
	    {% else %}
        $v = unpack("S*" , substr($memData , $offset, 2*{{ sc.Len }}));
	    {% endif %}
	    {% if sc.Data is defined %}
	    {% for d in sc.Data %}
	    {% if d.Value is defined %}
    $dataArray["{{ d.Name }}"] = {{ d.Value }};
	    {% else %}
    $dataArray["{{ d.Name }}"] = number_format($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
	    {% endif %}
        {% if d.Alias is defined %}
        {% for newName in d.Alias %}
        $dataArray["{{ newName }}"] = $dataArray["{{ d.Name }}"];
        {% endfor %}
        {% endif %}
	    {% endfor %}
	    {% endif %}
	$offset += {{ 2*sc.Len }};
	    {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
        $v = unpack("C*" , substr($memData , $offset, {{ sc.Len }} ));
	    {% if sc.Data is defined %}
	    {% for d in sc.Data %}
	$dataArray["{{ d.Name }}"] = number_format($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], 2);    
	    {% endfor %}
	    {% endif %}
	$offset += {{ sc.Len }};
            {% endif %}
        {% endfor %}
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

