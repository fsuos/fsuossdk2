import PyFsuos2
import numpy as np
class PyDTSD3366D(PyFsuos2.PyModbusImp):
    DTSD3366D_R3_9=101
    DTSD3366D_R3_366=102
    DTSD3366D_R3_378=103
    DTSD3366D_R3_402=104
    DTSD3366D_R3_4096=105
    DTSD3366D_R3_4144=106
    DTSD3366D_R3_4192=107
    def __init__(self,*args, **kwargs):
        super().__init__(*args, **kwargs)
        self.r3_9 = np.zeros(2,np.uint16)
        self.r3_366 = np.zeros(12,np.uint16)
        self.r3_378 = np.zeros(24,np.uint16)
        self.r3_402 = np.zeros(8,np.uint16)
        self.r3_4096 = np.zeros(2,np.uint16)
        self.r3_4144 = np.zeros(2,np.uint16)
        self.r3_4192 = np.zeros(2,np.uint16)
        self.state = 100
    def InitSetting(self, settingRoot):
        print("InitSetting")

    def RunCheckThreshold(self):
        print("RunCheckThreshold")
        RawData = self.r3_9.tobytes() + self.r3_366.tobytes() + self.r3_378.tobytes() + self.r3_402.tobytes() + self.r3_4096.tobytes() + self.r3_4144.tobytes() + self.r3_4192.tobytes()
        self.set_cdata(RawData)

    def process_payload(self, type, len):
        if self.state == PyDTSD3366D.DTSD3366D_R3_9:
            self.r3_9 = np.copy(self.tab_reg[0:2])
            self.state = PyDTSD3366D.DTSD3366D_R3_366
            self.modbus_read_registers(366, 12)
        elif self.state == PyDTSD3366D.DTSD3366D_R3_366:
            self.r3_366 = np.copy(self.tab_reg[0:12])
            self.state = PyDTSD3366D.DTSD3366D_R3_378
            self.modbus_read_registers(378, 24)
        elif self.state == PyDTSD3366D.DTSD3366D_R3_378:
            self.r3_378 = np.copy(self.tab_reg[0:24])
            self.state = PyDTSD3366D.DTSD3366D_R3_402
            self.modbus_read_registers(402, 8)
        elif self.state == PyDTSD3366D.DTSD3366D_R3_402:
            self.r3_402 = np.copy(self.tab_reg[0:8])
            self.state = PyDTSD3366D.DTSD3366D_R3_4096
            self.modbus_read_registers(4096, 2)
        elif self.state == PyDTSD3366D.DTSD3366D_R3_4096:
            self.r3_4096 = np.copy(self.tab_reg[0:2])
            self.state = PyDTSD3366D.DTSD3366D_R3_4144
            self.modbus_read_registers(4144, 2)
        elif self.state == PyDTSD3366D.DTSD3366D_R3_4144:
            self.r3_4144 = np.copy(self.tab_reg[0:2])
            self.state = PyDTSD3366D.DTSD3366D_R3_4192
            self.modbus_read_registers(4192, 2)
        elif self.state == PyDTSD3366D.DTSD3366D_R3_4192:
            self.r3_4192 = np.copy(self.tab_reg[0:2])
            self.RoundDone()
            return False
   
        return True;

    def RefreshStatus(self):
        print("RefreshStatus")
        self.state = PyDTSD3366D.DTSD3366D_R3_9;
        self.modbus_read_registers(9, 2);
        return True

    def Get_Value(self, data_id, var_name):
        print("Get_Value")

    def DeviceIoControl(self, ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned):
        print("DeviceIoControl")
