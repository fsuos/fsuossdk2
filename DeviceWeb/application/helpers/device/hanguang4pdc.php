<?php
function _hanguang4pdc_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_hanguang4pdc_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
 	$mem = substr($memData, $offset, 2*12);
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("f*" , $newMem);
    $dataArray["电压"] = number_format($v[1], 2)."V";
        $dataArray["电压1"] = $dataArray["电压"];
        $dataArray["”电压2""] = $dataArray["电压"];
        $dataArray["电压3"] = $dataArray["电压"];
    $dataArray["第1回路电流"] = number_format($v[3], 2)."A";
    $dataArray["第1回路功率"] = number_format($v[4], 2)."kW";
    $dataArray["第1回路电能"] = number_format($v[5], 2)."kWh";
    $dataArray["第1回路电能占比"] = number_format($v[6], 2)."%";
	$offset += 24;
 	$mem = substr($memData, $offset, 2*10);
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("f*" , $newMem);
    $dataArray["第2回路电流"] = number_format($v[2], 2)."A";
    $dataArray["第2回路功率"] = number_format($v[3], 2)."kW";
    $dataArray["第2回路电能"] = number_format($v[4], 2)."kWh";
    $dataArray["第2回路电能占比"] = number_format($v[5], 2)."%";
	$offset += 20;
 	$mem = substr($memData, $offset, 2*10);
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("f*" , $newMem);
    $dataArray["第3回路电流"] = number_format($v[2], 2)."A";
    $dataArray["第3回路功率"] = number_format($v[3], 2)."kW";
    $dataArray["第3回路电能"] = number_format($v[4], 2)."kWh";
    $dataArray["第3回路电能占比"] = number_format($v[5], 2)."%";
	$offset += 20;
 	$mem = substr($memData, $offset, 2*10);
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("f*" , $newMem);
    $dataArray["第4回路电流"] = number_format($v[2], 2)."A";
    $dataArray["第4回路功率"] = number_format($v[3], 2)."kW";
    $dataArray["第4回路电能"] = number_format($v[4], 2)."kWh";
    $dataArray["第4回路电能占比"] = number_format($v[5], 2)."%";
	$offset += 20;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

