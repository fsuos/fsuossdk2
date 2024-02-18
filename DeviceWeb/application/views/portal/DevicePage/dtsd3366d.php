<h4>系统数据</h4>
<?php $signalList = [
            "电压变比",
            "电流变比",
            "A相电压",
            "B相电压",
            "C相电压",
            "A相电流",
            "B相电流",
            "C相电流",
            "瞬时总有功有功率",
            "A相有功功率",
            "B相有功功率",
            "C相有功功率",
            "瞬时总无功有功率",
            "A相无功功率",
            "B相无功功率",
            "C相无功功率",
            "瞬时总视在功率",
            "A相视在功率",
            "B相视在功率",
            "C相视在功率",
            "总功率因数字",
            "A相功率因数字",
            "B相功率因数字",
            "C相功率因数字",
            "A相相角字",
            "B相相角字",
            "C相相角字",
            "频率",
            "组合有功电能",
            "正向有功电能",
            "反向有功电能",
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


