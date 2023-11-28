/*
 * Copyright © 2001-2013 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef JGCX_MODBUS_H
#define JGCX_MODBUS_H

/*
 *  ---------- Request     Indication ----------
 *  | Client | ---------------------->| Server |
 *  ---------- Confirmation  Response ----------
 */
typedef enum {
	/* Request message on the server side */
	MSG_INDICATION,
	/* Request message on the client side */
	MSG_CONFIRMATION
} jgcx_msg_type_t;

/* Add this for macros that defined unix flavor */
#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif

#ifndef _MSC_VER
#include <stdint.h>
#else
#include "stdint.h"
#endif

#include "jgcx_modbus-version.h"

#if defined(_MSC_VER)
# if defined(DLLBUILD)
/* define DLLBUILD when building the DLL */
#  define MODBUS_API __declspec(dllexport)
# else
#  define MODBUS_API __declspec(dllimport)
# endif
#else
# define MODBUS_API
#endif

#ifdef  __cplusplus
# define MODBUS_BEGIN_DECLS  extern "C" {
# define MODBUS_END_DECLS    }
#else
# define MODBUS_BEGIN_DECLS
# define MODBUS_END_DECLS
#endif

MODBUS_BEGIN_DECLS

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

/* Modbus function codes */
#define JGCX_MODBUS_FC_READ_COILS                0x01
#define JGCX_MODBUS_FC_READ_DISCRETE_INPUTS      0x02
#define MODBUS_FC_READ_DISCRETE_INPUTS      0x02
#define JGCX_MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define JGCX_MODBUS_FC_READ_INPUT_REGISTERS      0x04
#define JGCX_MODBUS_FC_WRITE_SINGLE_COIL         0x05
#define JGCX_MODBUS_FC_WRITE_SINGLE_REGISTER     0x06
#define JGCX_MODBUS_FC_READ_EXCEPTION_STATUS     0x07
#define JGCX_MODBUS_FC_WRITE_MULTIPLE_COILS      0x0F
#define JGCX_MODBUS_FC_WRITE_MULTIPLE_REGISTERS  0x10
#define JGCX_MODBUS_FC_REPORT_SLAVE_ID           0x11
#define JGCX_MODBUS_FC_MASK_WRITE_REGISTER       0x16
#define JGCX_MODBUS_FC_WRITE_AND_READ_REGISTERS  0x17

#define JGCX_MODBUS_BROADCAST_ADDRESS    0

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define JGCX_MODBUS_MAX_READ_BITS              2000
#define JGCX_MODBUS_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define JGCX_MODBUS_MAX_READ_REGISTERS          125
#define JGCX_MODBUS_MAX_WRITE_REGISTERS         123
#define JGCX_MODBUS_MAX_WR_WRITE_REGISTERS      121
#define JGCX_MODBUS_MAX_WR_READ_REGISTERS       125

/* The size of the MODBUS PDU is limited by the size constraint inherited from
 * the first MODBUS implementation on Serial Line network (max. RS485 ADU = 256
 * bytes). Therefore, MODBUS PDU for serial line communication = 256 - Server
 * address (1 byte) - CRC (2 bytes) = 253 bytes.
 */
#define JGCX_MODBUS_MAX_PDU_LENGTH              253

/* Consequently:
 * - RTU MODBUS ADU = 253 bytes + Server address (1 byte) + CRC (2 bytes) = 256
 *   bytes.
 * - TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes.
 * so the maximum of both backend in 260 bytes. This size can used to allocate
 * an array of bytes to store responses and it will be compatible with the two
 * backends.
 */
#define JGCX_MODBUS_MAX_ADU_LENGTH              260

/* Random number to avoid errno conflicts */
#define JGCX_MODBUS_ENOBASE 112345678

/* Protocol exceptions */
enum {
	JGCX_MODBUS_EXCEPTION_ILLEGAL_FUNCTION = 0x01,
	JGCX_MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS,
	JGCX_MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE,
	JGCX_MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE,
	JGCX_MODBUS_EXCEPTION_ACKNOWLEDGE,
	JGCX_MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY,
	JGCX_MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE,
	JGCX_MODBUS_EXCEPTION_MEMORY_PARITY,
	JGCX_MODBUS_EXCEPTION_NOT_DEFINED,
	JGCX_MODBUS_EXCEPTION_GATEWAY_PATH,
	JGCX_MODBUS_EXCEPTION_GATEWAY_TARGET,
	JGCX_MODBUS_EXCEPTION_MAX
};

#define JGCX_JGCX_EMBXILFUN  (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_ILLEGAL_FUNCTION)
#define JGCX_EMBXILADD  (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS)
#define JGCX_EMBXILVAL  (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE)
#define JGCX_EMBXSFAIL  (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE)
#define JGCX_EMBXACK    (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_ACKNOWLEDGE)
#define JGCX_EMBXSBUSY  (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY)
#define JGCX_EMBXNACK   (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE)
#define JGCX_EMBXMEMPAR (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_MEMORY_PARITY)
#define JGCX_EMBXGPATH  (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_GATEWAY_PATH)
#define JGCX_EMBXGTAR   (JGCX_MODBUS_ENOBASE + JGCX_MODBUS_EXCEPTION_GATEWAY_TARGET)

/* Native libmodbus error codes */
#define EMBBADCRC  (JGCX_EMBXGTAR + 1)
#define EMBBADDATA (JGCX_EMBXGTAR + 2)
#define EMBBADEXC  (JGCX_EMBXGTAR + 3)
#define EMBUNKEXC  (JGCX_EMBXGTAR + 4)
#define EMBMDATA   (JGCX_EMBXGTAR + 5)
#define EMBBADSLAVE (JGCX_EMBXGTAR + 6)

extern const unsigned int libmodbus_jgcx_version_major;
extern const unsigned int libmodbus_jgcx_version_minor;
extern const unsigned int libmodbus_jgcx_version_micro;

typedef struct jgcx_modbus jgcx_modbus_t;


MODBUS_API int jgcx_modbus_set_slave(jgcx_modbus_t* ctx, int slave);

MODBUS_API void jgcx_modbus_free(jgcx_modbus_t *ctx);


uint8_t jgcx_compute_meta_length_after_function(int function,
        jgcx_msg_type_t msg_type);
int jgcx_compute_data_length_after_meta(jgcx_modbus_t *ctx, uint8_t *msg,
                                        jgcx_msg_type_t msg_type);
int jgcx_check_confirmation(jgcx_modbus_t *ctx, uint8_t *req,
                            uint8_t *rsp, int rsp_length, int *error_code);
#include "jgcx_modbus-rtu.h"

MODBUS_END_DECLS

#endif  /* MODBUS_H */
