#ifndef DIANZONGPROTOCOL_H
#define DIANZONGPROTOCOL_H

#include "SMDSPDevice.hpp"
#include "common.h"

struct pmbus_ac_40_46 {
    float vu;
    float vl;
    float iu;
    float fu;
    float fl;
    char  l;
    float p1;
};

//监控行标第三部分：智能设备通信协议》分散空调通信协议（YD/T1363.3-2005 B.13）
//PSM-a, 810G/B, Cybermate共用通讯协议
//电总协议=PMBus protocol Power Management Bus Protocol），即"电源管理总线协议" http://electronicdesign.com/power/basics-pmbus-design

class PMBusProtocol : public SMDSPDevice
{
public:
    PMBusProtocol();
    ~PMBusProtocol();
    bool process(uint8_t *data, size_t len) override;
    void write_pmbus_cmd(uint8_t cid1, uint8_t cid2);
    void write_pmbus_cmd(uint8_t cid1, uint8_t cid2, const uint8_t* data, int data_len);
    virtual bool process_payload() = 0;
    virtual bool process_error();
    PMBus_Header *pPMBus;

    virtual bool InitSetting(const Json::Value &settingRoot) override;
protected:
    int action_;
    uint8_t version_;
    //data是msg_buf的一半
    static const uint32_t MAX_BUF_SIZE = 8192;
    uint8_t msg_buf[MAX_BUF_SIZE] = {0};
    uint8_t msg_data[4096] = {0};
};

#endif // DIANZONGPROTOCOL_H
