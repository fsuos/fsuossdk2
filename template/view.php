{% macro render_sc(sc, scPrefix=none, signalList='$signalList') -%}
      {% if sc.Data is defined %}
      {% for d in sc.Data %}
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}({{signalList}}, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $i);
          }
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}({{signalList}}, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, {{ d.index }});
      {% else %}
        {% if d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          {{signalList}}[] = {% if scPrefix is not none %}"{{scPrefix}}".{% endif %}sprintf("{{ d.ArrayName }}", $i);
        }
        {% else %}
        {{signalList}}[] = {% if scPrefix is not none %}"{{scPrefix}}".{% endif %}"{{ d.Name }}";
        {% endif %}
      {%endif %}
      {% endfor %}
      {% endif %}
{%- endmacro %}
<?php
{% if BlockTemplate is defined %}
{% for key,blockDef in BlockTemplate.items() %}
if(!function_exists('_{{ Project.Name|lower }}_{{ key }}')){
function _{{ Project.Name|lower }}_{{ key }}(&$signalList, $prefix, $index{% if blockDef.HasIndex1 is defined  %},$index1{% endif %} {% if blockDef.HasIndex2 is defined %},$index2{% endif %})
{
    {% for d in blockDef.BlockContent %}
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($signalList, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $index + $i);
          }
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}($signalList, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $index {% if d.index is defined %}+ {{ d.index }}{% endif %} );
      {% else %}        
        {% if d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          $name = $prefix.sprintf("{{ d.ArrayName }}", strval($index + $i));
          $signalList[] = $name;
        }
        {% else %}
          $name = $prefix.sprintf("{{ d.Name }}", {% if d.Index is defined %}${{ d.Index }}{% else %}strval($index){% endif %});
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
            {% if Project.TabGroup is defined %}
              {% for name in Project.TabGroup %}
              echo '<li><a class="list" href="##">{{name}}</a></li>';
              {% endfor %}
            {% endif %}
            {% for tsc in Sample %}
              {% if tsc.TabGroup is defined %}
              {{ render_sc(tsc, tsc.TabGroup, "$groupSignalList") }}
              {% else %}
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
              {% endif %}
            {% endfor %}
            ?>
        </ul>
    </div>
    {% if Project.TabGroup is defined %}
      {% for name in Project.TabGroup %}
      <div class="tab-content">
      <?php $this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$groupSignalList["{{name}}"], "cols"=>6)); ?>
      </div>
      {% endfor %}
    {% endif %}
    
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
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($signalList, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $i);
          }
        {% endif %}
      {% elif d.Block is defined %}
      _{{ Project.Name|lower }}_{{ d.Block }}($signalList, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, {% if d.index is defined %}"{{ d.index }}"{% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %}{% else %}$index{% endif %});
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
      {% elif sc.CmdGroupStart is defined %}
        for($cgIndex = {{ sc.CmdGroupStart }},$index = 1; $cgIndex < {{ sc.CmdGroupEnd }}; $cgIndex+={{ sc.CmdGroupStep}}, $index++){
          {% if sc.CmdGroupPrefix is string %}
          $namePrefix = sprintf("{{ sc.CmdGroupPrefix }}", $index);
          {% else %}
          $namePrefixArray = {{ sc.CmdGroupPrefix }};
          $namePrefix = $namePrefixArray[$index-1];
          {% endif %}
          {% for ssc in sc.CmdGroupSample %}
          
          {{ render_sc(ssc, "$namePrefix", "$signalList") }}
          {% endfor %}
        }
      {% endif %}

{% endfor %}
$this->load->view("portal/DevicePage/signal_ctrl_noid", array("signalList"=>$signalList, "cols"=>6));
?>
{% endif %}

