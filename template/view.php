<?php
{% if BlockTemplate is defined %}
{% for key,blockDef in BlockTemplate.items() %}
function _{{ Project.Name|lower }}_{{ key }}(&$signalList, $index)
{
    {% for d in blockDef.BlockContent %}
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($signalList, $index + $i);
          }
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}($signalList, $index + {{ d.index }});
      {% else %}        
        {% if d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          $name = sprintf("{{ d.ArrayName }}", $index + $i);
          $signalList[] = $name;
        }
        {% else %}
          $name = sprintf("{{ d.Name }}", $index);
          $signalList[] = $name;
        {% endif %}
      {% endif %}
          {% if d.Alias is defined %}
          {% for newName in d.Alias %}
          $signalList[] = {{ newName }};
          {% endfor %}
          {% endif %}
    {% endfor %}
}
{% endfor %}
{% endif %}
?>
<h4>系统数据</h4>
<?php $signalList = [];
{% for sc in Sample %}
      {% if sc.Cmd == 3 or sc.Cmd == 4 %}
      {% if sc.Data is defined %}
      {% for d in sc.Data %}
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($signalList, $i);
          }
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}($signalList, {{ d.index }});
      {% else %}
        {% if d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          $signalList[] = sprintf("{{ d.ArrayName }}", $i);
        }
        {% else %}
        $signalList[] = "{{ d.Name }}";
        {% endif %}
      {%endif %}
      {% endfor %}
      {% endif %}
      {% endif %}
{% endfor %}
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>

<h4>系统状态</h4>
<?php $signalList = [];
{% for sc in Sample %}
            {% if sc.Cmd == 1 or sc.Cmd == 2 %}
            {% if sc.Data is defined %}
            {% for d in sc.Data %}
            {% if d.ArrayName is defined %}
            for($i=1;$i<={{ d.ArrayLength }};$i++){
              $signalList[] = sprintf("{{ d.ArrayName }}", $i);
            }
            {% else %}
            $signalList[] = "{{ d.Name }}";
            {% endif %}
            {% endfor %}
            {% endif %}
            {% endif %}
      {% endfor %}
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>


