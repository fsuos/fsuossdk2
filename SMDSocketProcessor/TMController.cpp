#include "common_define.h"
#include "TMController.h"
#include "UniDataDevice.cpp"
#include <future>

TMController::TMController()
{
    device_type_ = "tmcontroller";
    addr_ = 1;

}

TMController::~TMController()
{
    if(nullptr != pCtx){
        modbus_close(pCtx);
        modbus_free(pCtx);
    }
}

bool TMController::Init ( std::weak_ptr<SMDController> controller, unsigned int  data_id, const Json::Value& settingRoot )
{
    SMDSocketDevice::Init ( controller, data_id, settingRoot );
    return true;
}



static void copy_to_float(uint16_t* buf, uint8_t *pFloat)
{
	memcpy(pFloat, buf+1, 2);
	memcpy(pFloat+2, buf, 2);
}

void TMController::RunCheckThreshold()
{    
    switch(b_mode_)
    {
        case 1://联通
        {
			CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "气管阀故障", (cData.r4_0[45]&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "液管阀故障", ((cData.r4_0[45]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏仪故障", ((cData.r4_0[45]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏高", ((cData.r4_0[45]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "触摸屏通讯离线", ((cData.r4_0[45]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "储液罐低液位", ((cData.r4_0[45]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机电机温度1故障", ((cData.r4_0[45]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机电机温度2故障", ((cData.r4_0[45]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602432101", "压缩机电机高温告警", ((cData.r4_0[45]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机电机高温停机", ((cData.r4_0[45]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机吸气温度故障", ((cData.r4_0[45]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机排气温度故障", ((cData.r4_0[45]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机吸气高温", ((cData.r4_0[45]>>13)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机吸气低温", ((cData.r4_0[45]>>14)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机排气高温", ((cData.r4_0[45]>>15)&0x1), signal_index_++);



CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机排气低温", ((cData.r4_0[46])&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机吸气压力故障", ((cData.r4_0[46]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机排气压力故障", ((cData.r4_0[46]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "轴承排气压力故障", ((cData.r4_0[46]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机吸气高压", ((cData.r4_0[46]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602230D01", "602230D01", "压缩机吸气低压", ((cData.r4_0[46]>>5&0x1)), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602230E03", "602230E03", "压缩机排气高压", ((cData.r4_0[46]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机排气低压", ((cData.r4_0[46]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "轴承排气高压", ((cData.r4_0[46]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "轴承排气低压", ((cData.r4_0[46]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机高压比", ((cData.r4_0[46]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机驱动离线", ((cData.r4_0[46]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机高电流告警", ((cData.r4_0[46]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机高电流停机", ((cData.r4_0[46]>>13)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机驱动故障", ((cData.r4_0[46]>>14)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机急停", ((cData.r4_0[46]>>15)&0x1), signal_index_++);




CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "震动传感器故障", ((cData.r4_0[47])&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机高震动", ((cData.r4_0[47]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐压力故障", ((cData.r4_0[47]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐高压告警", ((cData.r4_0[47]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐高压停机", ((cData.r4_0[47]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机供气高压差", ((cData.r4_0[47]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "压缩机供气低压差", ((cData.r4_0[47]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐出口压力故障", ((cData.r4_0[47]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐液位故障", ((cData.r4_0[47]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐低液位", ((cData.r4_0[47]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气罐高液位", ((cData.r4_0[47]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气齿轮泵故障", ((cData.r4_0[47]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "供气电加热故障", ((cData.r4_0[47]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔水盘水温故障", ((cData.r4_0[47]>>13)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔水盘高水温", ((cData.r4_0[47]>>14)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔水盘低水温", ((cData.r4_0[47]>>15)&0x1), signal_index_++);





CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔出口温度故障", ((cData.r4_0[48])&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔出口高温", ((cData.r4_0[48]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔出口低温", ((cData.r4_0[48]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔出口压力故障", ((cData.r4_0[48]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔出口高压", ((cData.r4_0[48]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔出口低压", ((cData.r4_0[48]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔喷淋泵故障", ((cData.r4_0[48]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷凝风机驱动通讯故障", ((cData.r4_0[48]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷凝风机驱动故障", ((cData.r4_0[48]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔上回风温湿度离线", ((cData.r4_0[48]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔侧回风温湿度离线", ((cData.r4_0[48]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵1驱动离线", ((cData.r4_0[48]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵2驱动离线", ((cData.r4_0[48]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵1进口压力故障", ((cData.r4_0[48]>>13)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵2进口压力故障", ((cData.r4_0[48]>>14)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵1出口压力故障", ((cData.r4_0[48]>>15)&0x1), signal_index_++);






CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵2出口压力故障", ((cData.r4_0[49])&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵1驱动故障", ((cData.r4_0[49]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵2驱动故障", ((cData.r4_0[49]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵1高扬程", ((cData.r4_0[49]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵1低扬程", ((cData.r4_0[49]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵2高扬程", ((cData.r4_0[49]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "泵2低扬程", ((cData.r4_0[49]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔水槽低水位", ((cData.r4_0[49]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "主控通讯离线", ((cData.r4_0[49]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷却塔水槽高水位", ((cData.r4_0[49]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏仪通信故障", ((cData.r4_0[49]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源丢失", ((cData.r4_0[49]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "变送器1通信故障", ((cData.r4_0[49]>>13)&0x1), signal_index_++);






CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风高湿", (cData.r4_100[8]&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风低湿", ((cData.r4_100[8]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源故障(CR)", ((cData.r4_100[8]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "监控离线", ((cData.r4_100[8]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风低温", ((cData.r4_100[8]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风高温", ((cData.r4_100[8]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回风高温", ((cData.r4_100[8]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回风高湿", ((cData.r4_100[8]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回风低湿", ((cData.r4_100[8]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回风低温", ((cData.r4_100[8]>>13)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风温度2故障(CR)", ((cData.r4_100[8]>>14)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回温1故障", ((cData.r4_100[8]>>15)&0x1), signal_index_++);






CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回湿故障", ((cData.r4_100[9])&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风温度1故障", ((cData.r4_100[9]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "风机故障", ((cData.r4_100[9]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "冷凝高水位(CR)", ((cData.r4_100[9]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602432C01", "602432C01", "地板溢水", ((cData.r4_100[9]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602433001", "602433001", "远程关机", ((cData.r4_100[9]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "气流丢失", ((cData.r4_100[9]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源丢失", ((cData.r4_100[9]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "过滤网堵", ((cData.r4_100[9]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送风温度3故障(CR)", ((cData.r4_100[9]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "烟感告警", ((cData.r4_100[9]>>13)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "送湿故障(CM)", ((cData.r4_100[9]>>14)&0x1), signal_index_++);





CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "空调关机", (cData.r4_100[10]&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "显示通信故障", ((cData.r4_100[10]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "DI模块通信故障(CR)", ((cData.r4_100[10]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源模块全部故障(CR)", ((cData.r4_100[10]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "EEV2控制板故障（CM）", ((cData.r4_100[10]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "EEV2通信故障（CM）", ((cData.r4_100[10]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "吸气温度2故障（CM）", ((cData.r4_100[10]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "低压压力2故障（CM）", ((cData.r4_100[10]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "吸气温度1故障", ((cData.r4_100[10]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "吸气压力1故障", ((cData.r4_100[10]>>11)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回风温度2故障(CR)", ((cData.r4_100[10]>>12)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "回风温度3故障(CR)", ((cData.r4_100[10]>>13)&0x1), signal_index_++);








CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机1", ((cData.r4_100[16])&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机2", ((cData.r4_100[16]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机3(CR)", ((cData.r4_100[16]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机4(CR)", ((cData.r4_100[16]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机5(CR)", ((cData.r4_100[16]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机6(CR)", ((cData.r4_100[16]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机7(CR)", ((cData.r4_100[16]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机8(CR)", ((cData.r4_100[16]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机9(CR)", ((cData.r4_100[16]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机10(CR)", ((cData.r4_100[16]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机11(CR)", ((cData.r4_100[16]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602431C01", "602431C01", "风机12(CR)", ((cData.r4_100[16]>>11)&0x1), signal_index_++);



CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源故障1", ((cData.r4_400[0]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源故障2", ((cData.r4_400[0]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏-一般", ((cData.r4_400[0]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏-严重", ((cData.r4_400[0]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏仪1离线", ((cData.r4_400[0]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "制冷剂泄漏仪2离线", ((cData.r4_400[0]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "电源丢失", ((cData.r4_400[0]>>10)&0x1), signal_index_++);



CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管A0X离线", (cData.r4_400[1]&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管B0X离线", ((cData.r4_400[1]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管C0X离线", ((cData.r4_400[1]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管D0X离线", ((cData.r4_400[1]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管E0X离线", ((cData.r4_400[1]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管F0X离线", ((cData.r4_400[1]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管G0X离线", ((cData.r4_400[1]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管H0X离线", ((cData.r4_400[1]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管I0X离线", ((cData.r4_400[1]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管J0X离线", ((cData.r4_400[1]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "N#室外主机离线", ((cData.r4_400[1]>>10)&0x1), signal_index_++);




CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N01离线", (cData.r4_400[2]&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N02离线", ((cData.r4_400[2]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N03离线", ((cData.r4_400[2]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N04离线", ((cData.r4_400[2]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N05离线", ((cData.r4_400[2]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N06离线", ((cData.r4_400[2]>>5)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N01离线", ((cData.r4_400[2]>>6)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N02离线", ((cData.r4_400[2]>>7)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N03离线", ((cData.r4_400[2]>>8)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N04离线", ((cData.r4_400[2]>>9)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N05离线", ((cData.r4_400[2]>>10)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "热管N06离线", ((cData.r4_400[2]>>11)&0x1), signal_index_++);




CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "1#室内主控离线", (cData.r4_400[3]&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "2#室内主控离线", ((cData.r4_400[3]>>1)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "3#室内主控离线", ((cData.r4_400[3]>>2)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "4#室内主控离线", ((cData.r4_400[3]>>3)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "5#室内主控离线", ((cData.r4_400[3]>>4)&0x1), signal_index_++);
CheckThresholdBool(LEVEL_1, "reg_40131_0", "602400101", "602400101", "6#室内主控离线", ((cData.r4_400[3]>>5)&0x1), signal_index_++);
            break;
        }
        case 2://电信
        {

            break;
        }
    }
}

bool TMController::RefreshStatus()
{
   if(!ip_.empty())
    {
        if(state != IDLE)
        {
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            boost::posix_time::time_duration  diff = now - lastTime;
            if( diff.total_seconds() > 60) {
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
            }else{
                return false;
            }
        }
        state = 1;
        if(nullptr == pCtx){
            pCtx = modbus_new_tcp(ip_.c_str(), port_);
            std::async( std::launch::async,
                        [&]() {
                int nRet = modbus_connect(pCtx);
                if(-1 == nRet)
                {
                    //printf("connect failed\n");
                    modbus_close(pCtx);
                    modbus_free(pCtx);
                    pCtx = nullptr;
                    return;
                }
                uint16_t regs[125];
                uint8_t bits[2000];
                    if(-1 == modbus_read_input_registers(pCtx, 0, 51, regs)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.r4_0, regs, sizeof(uint16_t)*51);
                   
                    if(-1 == modbus_read_input_registers(pCtx, 100, 23, regs)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.r4_100, regs, sizeof(uint16_t)*23);
                   
                    if(-1 == modbus_read_input_registers(pCtx, 400, 7, regs)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.r4_400, regs, sizeof(uint16_t)*7);
                   
                    if(-1 == modbus_read_registers(pCtx, 0, 3, regs)){
                        modbus_close(pCtx);
                        modbus_free(pCtx);
                        pCtx = nullptr;
                        return;
                    }
                    memcpy(cData.r3_0, regs, sizeof(uint16_t)*3);
                   
                modbus_close(pCtx);
                modbus_free(pCtx);
                pCtx = nullptr;
                RoundDone();
                return;
            });
        }        
    }    
    return false;
}

bool TMController::process_data(tcp::socket::native_handle_type fd, uint8_t *buffer, int size)
{
        return true;
}

float TMController::Get_Value(uint32_t data_id, const std::string& var_name) const
{
    if(!bIsDataReady_)
        throw std::out_of_range("数据未就绪");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - lastTime;
    if( diff.total_seconds() > 60) {
        throw std::out_of_range("数据已超时");
    }
    throw std::out_of_range("不支持变量");
}



int TMController::DeviceIoControl(int ioControlCode, void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned)
{
    switch(ioControlCode) {
	case 310:{
	  //SET_AODATA
        std::string aoStr((char*)inBuffer, inBufferSize);
        Json::Value setting;//
        Json::Reader reader;//解析
        if(!reader.parse(aoStr, setting)) {
          return -1;
        }
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
            if(setting["signal_id"].asString() != "") {
            }
        }
    }
    break;
    case 320: {
        state = 320;
        cmd_result_ = -1;
        //320是SET_DO
        std::string aoStr((char*)inBuffer, inBufferSize);
        Json::Value setting;//
        Json::Reader reader;//解析
        if(!reader.parse(aoStr, setting)) {
          return -1;
        }
        if(setting["signal_id"] != Json::nullValue && setting["signal_id"].type()  != Json::nullValue) {
            if(setting["signal_id"].asString() != "") {
            }
        }
        break;
    }

    default:
        if((uint32_t)outBufferSize >= sizeof(int)) {
            *((int*)outBuffer) = 2;//无效命令
            bytesReturned = sizeof(int);
        } else {
            bytesReturned = 0;
        }
        break;
    }
    return UniDataDevice<TMController_Data_t, SMDSocketDevice, RT_TMController>::DeviceIoControl(ioControlCode, inBuffer, inBufferSize, outBuffer, outBufferSize, bytesReturned);
}


#ifdef USE_SEPERATE_DRIVER

extern "C"
std::vector<std::shared_ptr<Provider>> get_providers()
{
    std::vector<std::shared_ptr<Provider>> providerVec;
    providerVec.push_back(std::make_shared<TMControllerProvider>());
    return std::move(providerVec);
}

#endif
