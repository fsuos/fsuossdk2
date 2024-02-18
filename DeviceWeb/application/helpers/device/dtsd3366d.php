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
        $v = unpack("S*" , substr($memData , $offset, 2*2));
    $dataArray["电压变比"] = number_format($v[1], 2);
    $dataArray["电流变比"] = number_format($v[2], 2);
	$offset += 4;
 	    $mem = substr($memData, $offset, 2*12);
        $newMem = '';
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("I*" , $newMem);
    $dataArray["A相电压"] = number_format($v[1]/10000, 2)."V";
    $dataArray["B相电压"] = number_format($v[2]/10000, 2)."V";
    $dataArray["C相电压"] = number_format($v[3]/10000, 2)."V";
    $dataArray["A相电流"] = number_format($v[4]/10000, 2)."A";
    $dataArray["B相电流"] = number_format($v[5]/10000, 2)."A";
    $dataArray["C相电流"] = number_format($v[6]/10000, 2)."A";
	$offset += 24;
 	    $mem = substr($memData, $offset, 2*24);
        $newMem = '';
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("i*" , $newMem);
    $dataArray["瞬时总有功有功率"] = number_format($v[1]/10000, 2)."kW";
    $dataArray["A相有功功率"] = number_format($v[2]/10000, 2)."kW";
    $dataArray["B相有功功率"] = number_format($v[3]/10000, 2)."kW";
    $dataArray["C相有功功率"] = number_format($v[4]/10000, 2)."kW";
    $dataArray["瞬时总无功有功率"] = number_format($v[5]/10000, 2)."kvar";
    $dataArray["A相无功功率"] = number_format($v[6]/10000, 2)."kvar";
    $dataArray["B相无功功率"] = number_format($v[7]/10000, 2)."kvar";
    $dataArray["C相无功功率"] = number_format($v[8]/10000, 2)."kvar";
    $dataArray["瞬时总视在功率"] = number_format($v[9]/10000, 2)."kVA";
    $dataArray["A相视在功率"] = number_format($v[10]/10000, 2)."kVA";
    $dataArray["B相视在功率"] = number_format($v[11]/10000, 2)."kVA";
    $dataArray["C相视在功率"] = number_format($v[12]/10000, 2)."kVA";
	$offset += 48;
        $v = unpack("s*" , substr($memData , $offset, 2*8));
    $dataArray["总功率因数字"] = number_format($v[1]/1000, 2);
    $dataArray["A相功率因数字"] = number_format($v[2]/1000, 2);
    $dataArray["B相功率因数字"] = number_format($v[3]/1000, 2);
    $dataArray["C相功率因数字"] = number_format($v[4]/1000, 2);
    $dataArray["A相相角字"] = number_format($v[5]/10, 2)."°";
    $dataArray["B相相角字"] = number_format($v[6]/10, 2)."°";
    $dataArray["C相相角字"] = number_format($v[7]/10, 2)."°";
    $dataArray["频率"] = number_format($v[8]/100, 2)."Hz";
	$offset += 16;
 	    $mem = substr($memData, $offset, 2*2);
        $newMem = '';
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("i*" , $newMem);
    $dataArray["组合有功电能"] = number_format($v[1]/100, 2)."kWh";
	$offset += 4;
 	    $mem = substr($memData, $offset, 2*2);
        $newMem = '';
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("i*" , $newMem);
    $dataArray["正向有功电能"] = number_format($v[1]/100, 2)."kWh";
	$offset += 4;
 	    $mem = substr($memData, $offset, 2*2);
        $newMem = '';
        for ($i = 0; $i < strlen($mem); $i += 4) {
            $newMem .= $mem[$i + 2] . $mem[$i + 3] . $mem[$i] . $mem[$i + 1];
        }
        $v = unpack("i*" , $newMem);
    $dataArray["反向有功电能"] = number_format($v[1]/100, 2)."kWh";
	$offset += 4;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

