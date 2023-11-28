<?php
function _dtsd3366d_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_dtsd3366d_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
        $v = unpack("S*" , substr($memData , $offset, 2*4));
	$dataArray["A相电压"] = number_format($v[1], 2);    
	$dataArray["B相电压"] = number_format($v[3], 2);    
	$offset += 8;
        $v = unpack("S*" , substr($memData , $offset, 2*44));
	$offset += 88;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

