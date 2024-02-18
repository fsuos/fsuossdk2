<?php
function _agc150_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

function Get_agc150_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
	
	$offset = 4;
        $v = unpack("S*" , substr($memData , $offset, 2*27));
    $dataArray["发电机电压L1-L2"] = number_format($v[1], 2)."V";
    $dataArray["发电机电压L2-L3"] = number_format($v[2], 2)."V";
    $dataArray["发电机电压L3-L1"] = number_format($v[3], 2)."V";
    $dataArray["发电机电压L1-N"] = number_format($v[4], 2)."V";
    $dataArray["发电机电压L2-N"] = number_format($v[5], 2)."V";
    $dataArray["发电机电压L3-N"] = number_format($v[6], 2)."V";
    $dataArray["发电机频率L1"] = number_format($v[7]/100, 2)."Hz";
    $dataArray["发电机频率L2"] = number_format($v[8]/100, 2)."Hz";
    $dataArray["发电机频率L3"] = number_format($v[9]/100, 2)."Hz";
    $dataArray["发电机电压相角L1-L2"] = number_format($v[10]/10, 2)."度";
    $dataArray["发电机电压相角L2-L3"] = number_format($v[11]/10, 2)."度";
    $dataArray["发电机电压相角L3-L1"] = number_format($v[12]/10, 2)."度";
    $dataArray["发电机电流L1"] = number_format($v[13], 2)."A";
    $dataArray["发电机电流L2"] = number_format($v[14], 2)."A";
    $dataArray["发电机电流L3"] = number_format($v[15], 2)."A";
    $dataArray["发电机功率L1"] = number_format($v[16], 2)."kW";
    $dataArray["发电机功率L2"] = number_format($v[17], 2)."kW";
    $dataArray["发电机功率L3"] = number_format($v[18], 2)."kW";
    $dataArray["发电机功率"] = number_format($v[19], 2)."kW";
    $dataArray["发电机无功功率L1"] = number_format($v[20], 2)."kVAr";
    $dataArray["发电机无功功率L2"] = number_format($v[21], 2)."kVAr";
    $dataArray["发电机无功功率L3"] = number_format($v[22], 2)."kVAr";
    $dataArray["发电机无功功率"] = number_format($v[23], 2)."kVAr";
    $dataArray["发电机视在功率L1"] = number_format($v[24], 2)."kVA";
    $dataArray["发电机视在功率L2"] = number_format($v[25], 2)."kVA";
    $dataArray["发电机视在功率L3"] = number_format($v[26], 2)."kVA";
    $dataArray["发电机视在功率"] = number_format($v[27], 2)."kVA";
	$offset += 54;
        $v = unpack("S*" , substr($memData , $offset, 2*13));
    $dataArray["发电机PF"] = number_format($v[1]/100, 2);
    $dataArray["母排电压 L1-L2"] = number_format($v[2], 2)."V";
    $dataArray["母排电压 L2-L3"] = number_format($v[3], 2)."V";
    $dataArray["母排电压 L3-L1"] = number_format($v[4], 2)."V";
    $dataArray["母排电压 L1-N"] = number_format($v[5], 2)."V";
    $dataArray["母排电压 L2-N"] = number_format($v[6], 2)."V";
    $dataArray["母排电压 L3-N"] = number_format($v[7], 2)."V";
    $dataArray["母排频率L1"] = number_format($v[8]/100, 2)."Hz";
    $dataArray["母排频率L2"] = number_format($v[9]/100, 2)."Hz";
    $dataArray["母排频率L3"] = number_format($v[10]/100, 2)."Hz";
    $dataArray["母排电压 相角 L1-L2"] = number_format($v[11]/10, 2)."度";
    $dataArray["母排电压 相角 L2-L3"] = number_format($v[12]/10, 2)."度";
    $dataArray["母排电压 相角 L3-L1"] = number_format($v[13]/10, 2)."度";
	$offset += 26;
        $v = unpack("S*" , substr($memData , $offset, 2*3));
    $dataArray["机组运行"] = ((cData.r4_1018[0] >> 6) & 0x1) ? "是" : "否"
    $dataArray["闭锁模式"] = (cData.r4_1018[1] & 0x1) ? "是" : "否"
    $dataArray["手动模式"] = ((cData.r4_1018[1] >> 1) & 0x1) ? "是" : "否"
    $dataArray["半自动模式"] = ((cData.r4_1018[1] >> 2) & 0x1) ? "是" : "否"
    $dataArray["自动模式"] = ((cData.r4_1018[1] >> 3) & 0x1) ? "是" : "否"
    $dataArray["测试模式"] = ((cData.r4_1018[1] >> 4) & 0x1) ? "是" : "否"
    $dataArray["通信错误"] = (cData.r4_1018[2] & 0x1) ? "是" : "否"
    $dataArray["警告/黄色报警"] = ((cData.r4_1018[2] >> 1) & 0x1) ? "是" : "否"
    $dataArray["停机/红色报警"] = ((cData.r4_1018[2] >> 2) & 0x1) ? "是" : "否"
    $dataArray["超速"] = ((cData.r4_1018[2] >> 3) & 0x1) ? "是" : "否"
    $dataArray["冷却水温度1"] = ((cData.r4_1018[2] >> 4) & 0x1) ? "是" : "否"
    $dataArray["冷却水温度2"] = ((cData.r4_1018[2] >> 5) & 0x1) ? "是" : "否"
    $dataArray["油压1"] = ((cData.r4_1018[2] >> 6) & 0x1) ? "是" : "否"
    $dataArray["油压2"] = ((cData.r4_1018[2] >> 7) & 0x1) ? "是" : "否"
    $dataArray["油温1"] = ((cData.r4_1018[2] >> 8) & 0x1) ? "是" : "否"
    $dataArray["油温2"] = ((cData.r4_1018[2] >> 9) & 0x1) ? "是" : "否"
    $dataArray["冷却水液位1"] = ((cData.r4_1018[2] >> 10) & 0x1) ? "是" : "否"
    $dataArray["冷却水液位2"] = ((cData.r4_1018[2] >> 11) & 0x1) ? "是" : "否"
    $dataArray["排气缸差温1"] = ((cData.r4_1018[2] >> 12) & 0x1) ? "是" : "否"
    $dataArray["排气缸差温2"] = ((cData.r4_1018[2] >> 13) & 0x1) ? "是" : "否"
	$offset += 6;
	$v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));

    }
}

