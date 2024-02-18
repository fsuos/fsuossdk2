<?php
function _evocybermatedxmodbus_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_evocybermatedxmodbus_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
        $v = unpack("S*" , substr($memData , $offset, 2*40));
    $dataArray["1#回风温度"] = number_format($v[1]/10, 2)."℃";
    $dataArray["2#回风温度"] = number_format($v[2]/10, 2)."℃";
    $dataArray["3#回风温度"] = number_format($v[3]/10, 2)."℃";
    $dataArray["4#回风温度"] = number_format($v[4]/10, 2)."℃";
    $dataArray["5#回风温度"] = number_format($v[5]/10, 2)."℃";
    $dataArray["1#出风温度"] = number_format($v[6]/10, 2)."℃";
    $dataArray["室外温度"] = number_format($v[9]/10, 2)."℃";
    $dataArray["1#回风湿度"] = number_format($v[14]/10, 2)."%";
    $dataArray["2#回风湿度"] = number_format($v[15]/10, 2)."%";
    $dataArray["3#回风湿度"] = number_format($v[16]/10, 2)."%";
    $dataArray["4#回风湿度"] = number_format($v[17]/10, 2)."%";
    $dataArray["5#回风湿度"] = number_format($v[18]/10, 2)."%";
    $dataArray["室外湿度"] = number_format($v[17]/10, 2)."%";
    $dataArray["出风湿度"] = number_format($v[18]/10, 2)."%";
	$offset += 80;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

