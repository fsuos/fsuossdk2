<?php
function _{{ Project.Name|lower }}_ShowAlert(&$dataArray, $label, $value, $vsValue)
{
    $dataArray[$label] = $value == $vsValue ? '正常' : '告警';
    $dataArray['AlertArray'][$label] = $value != $vsValue;
}

{% if BlockTemplate is defined %}
{% for key,blockDef in BlockTemplate.items() %}
function _{{ Project.Name|lower }}_{{ key }}(&$dataArray, $memData, $prefix, $index {% if blockDef.HasIndex1 is defined  %},$index1{% endif %} {% if blockDef.HasIndex2 is defined %},$index2{% endif %})
{
    $offset = 0;
    {% if blockDef.BlockType is defined %}
      {% if blockDef.BlockRType is defined %}
        {% if blockDef.BlockRType == 1 %}
        $lMemData = '';
        for($i=0;$i<strlen($memData);$i+=4){
          $lMemData .= $memData[$i+2].$memData[$i+3].$memData[$i].$memData[$i+1];
        }
        $v = unpack("{{blockDef.BlockType}}*" , $lMemData);
        {% elif blockDef.BlockRType == 2 %}
        {% endif %}
      {% endif %}
    {% else %}
    $v = unpack("S*" , $memData);
    {% endif %}
    {% for d in blockDef.BlockContent %}
      
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          //这个地方
          {% if d.Transform is defined and d.Length is defined and d.Transform == "bits" %}
          $lMemData = substr($memData, $offset, {{ d.Length }});
          $lOffset = 0;
          $v = unpack('C*', $lMemData);
          $lMemData = '';
          for($j=1;$j<=count($v);$j++)
          {
              for($k = 0; $k <8; $k++)
              {
                $lMemData .= pack("C", ($v[$j]>>$k)&0x1);
              }
          }
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($lMemData, $lOffset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($dataArray, $tMemData, $prefix, $index + $i);
            $lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          $offset += {{ d.Length }};
          {% else %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($memData, $offset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($dataArray, $tMemData, $prefix, $index + $i);
            $offset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          {% endif %}          
        {% endif %}
      {% elif d.Block is defined %}
      $lMemData = substr($memData, $offset, {{ BlockTemplate[d.Block]["BlockLength"] }});
      _{{ Project.Name|lower }}_{{ d.Block }}($dataArray, $lMemData, $prefix, "{{ d.index }}"{% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %});
      {% else %}
        $name = $prefix.sprintf("{{ d.Name }}", {% if d.Index is defined %}${{ d.Index }}{% else %}$index{% endif %});
        {% if d.Value is defined %}
          {% if d.AlertNormalValue is defined %}
          _{{ Project.Name|lower }}_ShowAlert($dataArray, $name, {{ d.Value }}, {{ d.AlertNormalValue }});
          {% elif d.Options is defined %}
          
          switch({{ d.Value }}){
          {% for item in d.Options %}
            case {{ item.Key }}:
            $dataArray[$name] = "{{ item.Value }}";
              break;
          {% endfor %}
            default:
            $dataArray[$name]  = "无效值";
              break;
          }
          {% else %}
          $dataArray[$name]  = {{ d.Value }};
          {% endif %}
          {% elif d.Options is defined %}
          switch($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]){
          {% for item in d.Options %}
            case {{ item.Key }}:
            $dataArray[$name] = "{{ item.Value }}";
              break;
          {% endfor %}
            default:
              $dataArray[$name] = "无效值";
              break;
          }
          {% elif d.AlertNormalValue is defined %}
          _{{ Project.Name|lower }}_ShowAlert($dataArray, $name, $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], {{ d.AlertNormalValue }});
        {% elif d.ArrayName is defined %}
        for($i=1;$i<={{ d.ArrayLength }};$i++){
          $name = $prefix.sprintf("{{ d.ArrayName }}", $index + $i);
          $kIndex = {{ d.Offset }} + $i;
          $dataArray[$name] = number_format($v[$kIndex]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
        }
        {% else %}
          $dataArray[$name] = number_format($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
        {% endif %}
      {% endif %}
          {% if d.Alias is defined %}
          {% for newName in d.Alias %}
          $dataArray["{{ newName }}"] = $dataArray["{{ d.Name }}"];
          {% endfor %}
          {% endif %}
    {% endfor %}
}
{% endfor %}
{% endif %}

function Get_{{ Project.Name|lower }}_RtData($memData, &$dataArray, $extraPara = false){
    if($memData == false){
        $dataArray['无数据'] = true;
        $dataArray['更新时间'] = '无';
        $dataArray['错误'] = '数据长度不一致';
    }else{
        $dataArray['无数据'] = false;
        $dataArray['AlertArray'] = array();
        $offset = 4;
	{% for sc in Sample %}
	    {% if sc.Data is defined %}
	    {% for d in sc.Data %}
      {% if d.ArrayBlock is defined %}
        {% if d.ArrayStart is defined %}
          {% if d.Transform is defined and d.Length is defined and d.Transform == "bits" %}
          $lMemData = substr($memData, $offset, {{ d.Length }});
          $lOffset = 0;
          $v = unpack('C*', $lMemData);
          $lMemData = '';
          for($j=1;$j<=count($v);$j++)
          {
              for($k = 0; $k <8; $k++)
              {
                $lMemData .= pack("C", ($v[$j]>>$k)&0x1);
              }
          }
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($lMemData, $lOffset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($dataArray, $tMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $index + $i);
            $lOffset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          $offset += {{ d.Length }};
          {% else %}
          for($i={{d.ArrayStart}};$i<={{d.ArrayEnd}};$i++)
          {
            $tMemData = substr($memData, $offset, {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }});            
            _{{ Project.Name|lower }}_{{ d.ArrayBlock }}($dataArray, $tMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, $index + $i);
            $offset += {{ BlockTemplate[d.ArrayBlock]["BlockLength"] }};
          }
          {% endif %}     

        {% endif %}
      {% elif d.Block is defined %}
      $lMemData = substr($memData, $offset, {{ BlockTemplate[d.Block]["BlockLength"] }});
      _{{ Project.Name|lower }}_{{ d.Block }}($dataArray, $lMemData, {% if d.Prefix is defined %}"{{d.Prefix}}"{%else%}""{%endif%}, "{{ d.index }}" {% if d.index1 is defined %},"{{d.index1}}"{% endif %}{% if d.index2 is defined %},"{{d.index2}}"{% endif %} );
      $offset += {{ BlockTemplate[d.Block]["BlockLength"] }};
      {% else %}
      {% if sc.Type is defined %}
      $v = unpack("{{sc.Type}}*" , substr($memData , $offset, 2*{{ sc.Len }}));
    {% elif sc.Cmd == 3 or sc.Cmd == 4 %}
      $v = unpack("S*" , substr($memData , $offset, 2*{{ sc.Len }}));
    {% else %}
      $v = unpack("C*" , substr($memData , $offset, {{ sc.Len }}));
    {% endif %}
	    {% if d.Value is defined %}
        {% if d.AlertNormalValue is defined %}
        _{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ d.Name }}", {{ d.Value }}, {{ d.AlertNormalValue }});
        {% elif d.Options is defined %}
        switch({{ d.Value }}){
        {% for item in d.Options %}
          case {{ item.Key }}:
          $dataArray["{{ d.Name }}"] = "{{ item.Value }}";
            break;
        {% endfor %}
          default:
            $dataArray["{{ d.Name }}"] = "无效值";
            break;
        }
        {% else %}
        $dataArray["{{ d.Name }}"] = ({{ d.Value }}){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
        {% endif %}
        {% elif d.Options is defined %}
        switch($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]){
        {% for item in d.Options %}
          case {{ item.Key }}:
          $dataArray["{{ d.Name }}"] = "{{ item.Value }}";
          {% if item.IsAlert is defined and item.IsAlert %}
          $dataArray["AlertArray"]["{{ d.Name }}"] = 1;
          {% endif %}
            break;
        {% endfor %}
          default:
            $dataArray["{{ d.Name }}"] = "无效值";
            break;
        }
        {% elif d.AlertNormalValue is defined %}
        _{{ Project.Name|lower }}_ShowAlert($dataArray, "{{ d.Name }}", $v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}], {{ d.AlertNormalValue }});
      {% elif d.ArrayName is defined %}
      for($i=1;$i<={{ d.ArrayLength }};$i++){
        $name = sprintf("{{ d.ArrayName }}", $i);
        $kIndex = {{ d.Offset }} + $i;
        $dataArray[$name] = number_format($v[$kIndex]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
      }
	    {% else %}
        $dataArray["{{ d.Name }}"] = number_format($v[{% if d.Offset is defined %}{{ d.Offset }}{% else %}{{ loop.index }}{% endif %}]{% if d.Ratio is defined %}/{{ d.Ratio }}{% endif %}, 2){% if d.Unit is defined %}."{{ d.Unit }}"{% endif %};
	    {% endif %}
      {% if sc.Cmd == 3 or sc.Cmd == 4 %}
          $offset += {{ 2*sc.Len }};
        {% else %}
          $offset += {{ sc.Len }};
        {% endif %}
      {% endif %}
        {% if d.Alias is defined %}
        {% for newName in d.Alias %}
        $dataArray["{{ newName }}"] = $dataArray["{{ d.Name }}"];
        {% endfor %}
        {% endif %}

	    {% endfor %}
	    {% endif %}
        
        {% endfor %}
        $v = unpack('v', substr($memData, $offset , 2));
        $year = $v[1];
        $v = unpack('C*', substr($memData, $offset + 2 , 5));
        $dataArray['更新时间'] = date('Y-m-d H:i:s', strtotime($year . '-' . $v[1] . '-' . $v[2] . ' ' . $v[3] . ':' . $v[4] . ':' . $v[5]));
      {% if ExtraPhpCode is defined %}
      {{ ExtraPhpCode }}
      {% endif %}
    }
}

