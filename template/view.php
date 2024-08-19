{% macro render_sc(sc, scPrefix=none, signalList='$signalList') -%}
      {% if sc.Data is defined %}
      {% for d in sc.Data %}
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}({{signalList}}, $i);
          }
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}({{signalList}}, {{ d.index }});
      {% else %}
        {% if d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          {{signalList}}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}] = sprintf("{{ d.ArrayName }}", $i);
        }
        {% else %}
        {{signalList}}[{% if scPrefix is not none %}{{scPrefix}}][{% endif %}] = "{{ d.Name }}";
        {% endif %}
      {%endif %}
      {% endfor %}
      {% endif %}
{%- endmacro %}
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
{% if Project.TabView is defined and Project.TabView %}
<div class="tab-widget tabbable tabs-left chat-widget">
    <div class="tab-widget">
        <ul class="nav nav-tabs childTabs">
            <?php $signalList = [];
            $groupSignalList = [];
            {% for tsc in Sample %}
            {% if tsc.CmdGroupStart is defined %}
            for($cgIndex = {{ tsc.CmdGroupStart }},$index = 1; $cgIndex < {{ tsc.CmdGroupEnd }}; $cgIndex+={{ tsc.CmdGroupStep}}, $index++){
            {% if tsc.CmdGroupPrefix is string %}
            $namePrefix = sprintf("{{ tsc.CmdGroupPrefix }}", $index);
            {% else %}
            $namePrefixArray = {{ tsc.CmdGroupPrefix }};
            $namePrefix = $namePrefixArray[$index-1];
            {% endif %}
            {% for sc in tsc.CmdGroupSample %}
            {{ render_sc(sc, "$namePrefix", "$groupSignalList") }}
            {% endfor %}
            echo '<li><a class="list" href="##">'.$namePrefix.'</a></li>';
            }
            {% else %}
            {{ render_sc(tsc, none, "$signalList") }}
            {% endif %}
            {% endfor %}
            ?>
        </ul>
    </div>
    {% for tsc in Sample %}
    {% if tsc.CmdGroupStart is defined %}
    <?php for($cgIndex = {{ tsc.CmdGroupStart }},$index = 1; $cgIndex < {{ tsc.CmdGroupEnd }}; $cgIndex+={{ tsc.CmdGroupStep}}, $index++){
    {% if tsc.CmdGroupPrefix is string %}
    $namePrefix = sprintf("{{ tsc.CmdGroupPrefix }}", $index); 
    {% else %}
    $namePrefixArray = {{ tsc.CmdGroupPrefix }};
    $namePrefix = $namePrefixArray[$index-1]; 
    {% endif %} ?>
    <div class="tab-content">
    <?php $this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$groupSignalList[$namePrefix], "cols"=>6)); ?>
    </div>
    <?php } ?>
    {% endif %}
    {% endfor %}
  </div>
{% else %}
<?php $signalList = [];
{% for sc in Sample %}
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
          $signalList[] = sprintf("{{ d.ArrayName }}", {{ d.ArrayStart }} + $i);
        }
        {% else %}
        $signalList[] = "{{ d.Name }}";
        {% endif %}
      {%endif %}
      {% endfor %}
      {% endif %}
{% endfor %}
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>
{% endif %}

