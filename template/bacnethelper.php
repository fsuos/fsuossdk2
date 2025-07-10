<?php


function Get_{{ Project.Name|lower }}_RtData($memData, &$dataArray, $extraPara = false)
{
    if (strlen($memData) == false) {
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    } else {
        $dataArray['无数据'] = false;
        $offset = 4;
        {% for sc in Sample %}
        {% if sc.Cmd == 3 or sc.Cmd == 4 or sc.Cmd == 5 %}
        $v = unpack('C*', substr($memData, $offset, {{ sc.Offset|length }}));
        $signalList = array("{{ sc.Data|join('","') }}");
        foreach($signalList as $index=>$signal){
                $dataArray[$signal] = $v[$index+1];
        }
        $offset += {{ sc.Offset|length }};
        {% elif sc.Cmd == 0 or sc.Cmd == 1 or sc.Cmd == 2 %}
        $v = unpack('f*', substr($memData, $offset, 4*{{ sc.Offset|length }}));
        $signalList = array("{{ sc.Data|join('","') }}");
        foreach($signalList as $index=>$signal){
                $dataArray[$signal] = $v[$index+1];
        }
        $offset += 4*{{ sc.Offset|length }};
        {% elif sc.Cmd == 19 %}
        $v = unpack('I*', substr($memData, $offset, 4*{{ sc.Offset|length }}));
        $signalList = array("{{ sc.Data|join('","') }}");
        foreach($signalList as $index=>$signal){
                $dataArray[$signal] = $v[$index+1];
        }
        $offset += 4*{{ sc.Offset|length }};
        {% endif %}
        {% endfor %}
        $v = unpack('v', substr($memData, $offset, 2));
        $year = $v[1];
        $offset += 2;
        $v = unpack('C*', substr($memData, $offset, 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));
    }
}
