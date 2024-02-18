<?php
function _mediamav_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_mediamav_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
        $v = unpack("C*" , substr($memData , $offset, 114 ));
	$dataArray["内风机"] = number_format($v[48], 2);    
	$dataArray["压缩机1"] = number_format($v[49], 2);    
	$offset += 114;
        $v = unpack("S*" , substr($memData , $offset, 2*61));
    $dataArray["温度设点"] = number_format($v[1]/10, 2)."℃";
    $dataArray["湿度设点"] = number_format($v[3]/10, 2)."％";
    $dataArray["环境温度"] = number_format($v[49]/10, 2)."℃";
    $dataArray["环境湿度"] = number_format($v[50]/10, 2)."%";
    $dataArray["出风温度"] = number_format($v[51]/10, 2)."℃";
	$offset += 122;
        $v = unpack("S*" , substr($memData , $offset, 2*6));
    $dataArray["风机运行时间"] = number_format($v[1], 2)."H";
    $dataArray["压缩机1运行时间"] = number_format($v[2], 2)."H";
	$offset += 12;
        $v = unpack("S*" , substr($memData , $offset, 2*1));
    $dataArray["机组状态"] = ($v[1] == 0 || $v[1] == 0x0F) ? "开机" : "关机";
	$offset += 2;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

