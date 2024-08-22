#ifndef HBZS96K_H
#define HBZS96K_H
#include "PMBusProtocol.h"
#include "HBZS96K_interface.h"
#include "UniDataDevice.h"

#define RT_HBZS96K 5031
class HBZS96K: public UniDataDevice<HBZS96K_data_t, PMBusProtocol, RT_HBZS96K>
{
public:
    HBZS96K();
    bool RefreshStatus() override;
    bool process_payload() override;
    float Get_Value ( uint32_t data_id, const std::string &var_name ) const override;
    int DeviceIoControl(int ioControlCode, const void* inBuffer, int inBufferSize, void* outBuffer, int outBufferSize, int& bytesReturned) override;
    void RunCheckThreshold() override;
protected:
    enum HBZS96K_RTN {
        NORMAL       = 0x00,
        VER_MISMATCH = 0x01,
        CHKSUM_ERR   = 0x02,
        LCHKSUM_ERR  = 0x03,
        CID2_INVALID = 0x04,
        CMD_ERR      = 0x05,
        DATA_INVALID = 0x06,
        OTHER_ERROR  = 0x80
    };
    typedef enum {
        DATAMATE_IDLE = 0,
        RST_ALARM  = 0x40,
        GET_AI     = 0x42,
        GET_STATUS = 0x43,
        GET_ALARM  = 0x44,
        REMOTE_CTL = 0x45,
        GET_PARAMS = 0x47,
        SET_PARAMS = 0x49,
        GET_ADDR   = 0x50
    } DATAMATE3000_CMD;

    enum { STATE_IDLE, STATE_2c_41,STATE_2c_43, STATE_2c_44};

private:
    bool skipNext;

    int cmd_result_ = -1;
    uint16_t last_data_ = -1;
};

PLUMA_INHERIT_PROVIDER ( DataMate3000, SMDSPDevice );

#endif//CYBERMATE_H
