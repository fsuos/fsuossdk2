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
	{% if Sample is defined %}
        $v = unpack("C" , substr($memData , $offset, 1));
	{% if Sample.Name is defined and Sample.Data is defined %}
	_{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ Sample.Name }}", $v[1], {{ Sample.Data }});
	{% endif %}
	{% endif %}
	$offset = 5;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

