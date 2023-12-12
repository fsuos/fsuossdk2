<h4>系统数据</h4>
<?php $signalList = [
{% if Sample is defined %}
            {% if Sample.Name is defined %}
            "{{ Sample.Name }}",
            {% endif %}
      {% endif %}
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>2));
?>
