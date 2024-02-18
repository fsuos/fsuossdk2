<h4>系统数据</h4>
<?php $signalList = [
            "温度设点",
            "湿度设点",
            "环境温度",
            "环境湿度",
            "出风温度",
            "风机运行时间",
            "压缩机1运行时间",
            "机组状态",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
            "内风机",
            "压缩机1",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


