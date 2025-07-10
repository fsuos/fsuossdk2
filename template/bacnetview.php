{% for sc in Sample %}  
<?php
    $signalList = ["{{ sc.Data|join('","') }}"];
    $this->load->view("portal/DevicePage/signal_ctrl", array("signalList"=>$signalList, "cols"=>3));
    ?>
{% endfor %}
