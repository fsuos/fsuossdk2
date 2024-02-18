<h4>系统数据</h4>
<?php $signalList = [
            "电压",
            "第1回路电流",
            "第1回路功率",
            "第1回路电能",
            "第1回路电能占比",
            "第2回路电流",
            "第2回路功率",
            "第2回路电能",
            "第2回路电能占比",
            "第3回路电流",
            "第3回路功率",
            "第3回路电能",
            "第3回路电能占比",
            "第4回路电流",
            "第4回路功率",
            "第4回路电能",
            "第4回路电能占比",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


