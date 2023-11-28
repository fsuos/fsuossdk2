<h4>系统数据</h4>
<?php $signalList = [
{% for sc in Sample %}
            {% if sc.Cmd == 3 or sc.Cmd == 4 %}
            {% if sc.Data is defined %}
            {% for d in sc.Data %}
            "{{ d.Name }}",
            {% endfor %}
            {% endif %}
            {% endif %}
      {% endfor %}
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [
{% for sc in Sample %}
            {% if sc.Cmd == 1 or sc.Cmd == 2 %}
            {% if sc.Data is defined %}
            {% for d in sc.Data %}
            "{{ d.Name }}",
            {% endfor %}
            {% endif %}
            {% endif %}
      {% endfor %}
];
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


