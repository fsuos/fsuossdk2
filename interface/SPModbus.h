#ifndef SPMODBUS_H
#define SPMODBUS_H
#include "SMDSPDevice.hpp"
#include "jgcx_modbus.h"
#include "jgcx_modbus-private.h"
#include "jgcx_modbus-rtu.h"
class SPModbus: public SMDSPDevice
{
public:
	enum MODBUS_DEFINE {MODBUS_REGISTER_LEN = 2};
	enum  tab_type { TAB_BIT, TAB_REG } ;
	SPModbus();
	~SPModbus();
	bool process(unsigned char* data, size_t len) override;

	void set_peer_addr(int addr)
	{
		jgcx_modbus_set_slave(ctx, addr);
	}


	/**
	 * @brief 传递数据到下游驱动，告知本次数据类型，和长度，下游负责越界检查
	 * @param type tab_bits|reg
	 * @param len, elements in tab_reg|bits
	 */
	virtual bool process_payload(enum tab_type type, size_t len) = 0;

	bool modbus_read_bits(int addr, int nb);
	bool modbus_read_input_bits(int addr, int  nb);
	bool modbus_read_input_registers(int addr, int nb);
	bool modbus_read_registers(int addr, int nb);

	int modbus_write_bit(int coil_addr, int status);

	bool modbus_write_register(int addr, int value);

	bool modbus_write_registers(int addr, int nb, const uint16_t *src);

	int modbus_write_bits(int addr, int nb, const uint8_t *src);

	virtual bool InitSetting(const Json::Value& settingRoot) override;
	virtual bool on_error()
	{
		printf("SPModbus get error\n");
		return false;
	}
protected:
	bool read_registers(int function, int addr, int nb);
	jgcx_modbus_t *ctx;
	uint8_t tab_bit[JGCX_MODBUS_MAX_READ_BITS];
	uint16_t tab_reg[JGCX_MODBUS_MAX_READ_REGISTERS];
	uint8_t req[_MIN_REQ_LENGTH];
	int rc;
	int function_;
	int reg_addr_;
	int nb_;
	uint32_t header_to_read_;
	uint32_t total_data_len_;
	int error_code;
};

#endif // SPMODBUS_H
