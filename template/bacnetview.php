<?php
    $signalList = [];
    {% for sc in Sample %}
        {% for d in sc.Data %}
        {% if d.Name is defined %}
        $signalList[] = "{{ d.Name }}";
        {% else %}
        $signalList[] = "{{ d }}";
        {% endif %}
        {% endfor %}
    {% endfor %}
    $this->load->view("portal/DevicePage/signal_ctrl", array("signalList"=>$signalList, "cols"=>3));
    ?>
