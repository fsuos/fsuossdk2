<h4>系统数据</h4>
<?php $signalList = [
            "相电压U1",
            "相电压U2",
            "相电压U3",
            "三相平均相电压",
            "线电压U12",
            "线电压U23",
            "线电压U31",
            "三相平均线电压",
            "第1相电流",
            "第2相电流",
            "第3相电流",
            "三相平均电流",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


