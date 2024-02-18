<?php
function _etpower_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_etpower_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
 	$mem = substr($memData, $offset, 2*24);
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("f*" , $newMem);
    $dataArray["相电压U1"] = number_format($v[1], 2)."V";
    $dataArray["相电压U2"] = number_format($v[2], 2)."V";
    $dataArray["相电压U3"] = number_format($v[3], 2)."V";
    $dataArray["三相平均相电压"] = number_format($v[4], 2)."V";
    $dataArray["线电压U12"] = number_format($v[5], 2)."V";
    $dataArray["线电压U23"] = number_format($v[6], 2)."V";
    $dataArray["线电压U31"] = number_format($v[7], 2)."V";
    $dataArray["三相平均线电压"] = number_format($v[8], 2)."V";
    $dataArray["第1相电流"] = number_format($v[9], 2)."A";
    $dataArray["第2相电流"] = number_format($v[10], 2)."A";
    $dataArray["第3相电流"] = number_format($v[11], 2)."A";
    $dataArray["三相平均电流"] = number_format($v[12], 2)."A";
	$offset += 48;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

