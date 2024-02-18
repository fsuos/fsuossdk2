<h4>系统数据</h4>
<?php $signalList = [
            "1#回风温度",
            "2#回风温度",
            "3#回风温度",
            "4#回风温度",
            "5#回风温度",
            "1#出风温度",
            "室外温度",
            "1#回风湿度",
            "2#回风湿度",
            "3#回风湿度",
            "4#回风湿度",
            "5#回风湿度",
            "室外湿度",
            "出风湿度",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


