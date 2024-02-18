import PyFsuos2
import numpy as np
class Py{{ Project.Name }}(PyFsuos2.PyModbusImp):
    {% for sc in Sample %}
    {{ Project.Name + "_R%d_%d"%(sc.Cmd,sc.Offset) }}={{100 + loop.index}}
	{% endfor %}
    def __init__(self,*args, **kwargs):
        super().__init__(*args, **kwargs)
    {% for sc in Sample %}
    {% if sc.Cmd == 3 or sc.Cmd == 4 %}
        self.{{  "r%d_%d"%(sc.Cmd,sc.Offset) }} = np.zeros({{sc.Len}},np.uint16)
    {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
        self.{{  "r%d_%d"%(sc.Cmd,sc.Offset) }} = np.zeros({{sc.Len}},np.uint8)
    {% endif %}
    {% endfor %}
        self.state = 100
    def InitSetting(self, settingRoot):
        print("InitSetting")

    def RunCheckThreshold(self):
        print("RunCheckThreshold")
        RawData = {% for sc in Sample %}{% if sc.Cmd == 3 or sc.Cmd == 4 %}self.{{  "r%d_%d"%(sc.Cmd,sc.Offset) }}.tobytes(){% elif sc.Cmd == 1 or sc.Cmd == 2 %}self.{{  "r%d_%d"%(sc.Cmd,sc.Offset) }}.tobytes(){% endif %}{% if loop.nextitem is defined %} + {% endif %}{% endfor %}

        self.set_cdata(RawData)

    def process_payload(self, type, len):
    {% for sc in Sample %}
      {% if loop.index == 1 %}
        if self.state == Py{{ Project.Name }}.{{ Project.Name + "_R%d_%d"%(sc.Cmd,sc.Offset) }}:
      {% else %}
        elif self.state == Py{{ Project.Name }}.{{ Project.Name + "_R%d_%d"%(sc.Cmd,sc.Offset) }}:
      {% endif %}
	    {% if sc.Cmd == 3 or sc.Cmd == 4 %}
            self.{{ "r%d_%d"%(sc.Cmd,sc.Offset) }} = np.copy(self.tab_reg[0:{{  sc.Len }}])
        {% elif sc.Cmd == 1 or sc.Cmd == 2 %}
	        self.{{ "r%d_%d"%(sc.Cmd,sc.Offset) }} = np.copy(self.tab_bit[0:{{  sc.Len }}])
	    {% endif %}
	    {% if loop.nextitem is defined %}
            self.state = Py{{ Project.Name }}.{{ Project.Name + "_R%d_%d"%(loop.nextitem.Cmd,loop.nextitem.Offset) }}
        {% if loop.nextitem.Cmd == 3 %}
            self.modbus_read_registers({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }})
        {% elif loop.nextitem.Cmd == 4 %}
            self.modbus_read_input_registers({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }})
        {% elif loop.nextitem.Cmd == 1 %}
            self.modbus_read_bits({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }})
        {% elif loop.nextitem.Cmd == 2 %}
            self.modbus_read_input_bits({{ loop.nextitem.Offset }}, {{ loop.nextitem.Len }})
        {% endif %}
	    {% else %}
            self.RoundDone()
            return False
        {% endif %}
      {% endfor %}
   
        return True;

    def RefreshStatus(self):
        print("RefreshStatus")
        {% for sc in Sample %}
        self.state = Py{{ Project.Name }}.{{ Project.Name + "_R%d_%d"%(sc.Cmd, sc.Offset) + ";" }}
    {% if sc.Cmd == 3 %}
        self.modbus_read_registers({{ sc.Offset }}, {{ sc.Len }});
    {% elif sc.Cmd == 4 %}
        self.modbus_read_input_registers({{ sc.Offset }}, {{ sc.Len }});
    {% elif sc.Cmd == 1 %}
        self.modbus_read_bits({{ sc.Offset }}, {{ sc.Len }});
    {% elif sc.Cmd == 2 %}
        self.modbus_read_input_bits({{ sc.Offset }}, {{ sc.Len }});
    {% endif %}
    {% break %}
    {% endfor %}
        return True

    def Get_Value(self, data_id, var_name):
        print("Get_Value")

    def DeviceIoControl(self, ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned):
        print("DeviceIoControl")
