<h4>系统数据</h4>
<?php $signalList = [
            "A相电压",
            "B相电压",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


