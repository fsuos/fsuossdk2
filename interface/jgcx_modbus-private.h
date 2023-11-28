/*
 * Copyright © 2010-2012 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef JGCX_MODBUS_PRIVATE_H
#define JGCX_MODBUS_PRIVATE_H

#ifndef _MSC_VER
# include <stdint.h>
# include <sys/time.h>
#else
# include "stdint.h"
# include <time.h>
typedef int ssize_t;
#endif
#include <sys/types.h>
#include <config.h>

#include "jgcx_modbus.h"

MODBUS_BEGIN_DECLS

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
 */
#define _MIN_REQ_LENGTH 12

#define _REPORT_SLAVE_ID 180

#define _MODBUS_EXCEPTION_RSP_LENGTH 5

/* Timeouts in microsecond (0.5 s) */
#define _RESPONSE_TIMEOUT    500000
#define _BYTE_TIMEOUT        500000

typedef enum {
	_JGCX_MODBUS_BACKEND_TYPE_RTU=0,
	_JGCX_MODBUS_BACKEND_TYPE_TCP
} jgcx_modbus_backend_type_t;



/* This structure reduces the number of params in functions and so
 * optimizes the speed of execution (~ 37%). */
typedef struct _jgcx_sft {
	int slave;
	int function;
	int t_id;
} jgcx_sft_t;

typedef struct _jgcx_modbus_backend {
	unsigned int backend_type;
	unsigned int header_length;
	unsigned int checksum_length;
	unsigned int max_adu_length;
	int (*set_slave) (jgcx_modbus_t *ctx, int slave);
	int (*build_request_basis) (jgcx_modbus_t *ctx, int function, int addr,
	                            int nb, uint8_t *req);
	int (*build_response_basis) (jgcx_sft_t *sft, uint8_t *rsp);
	int (*prepare_response_tid) (const uint8_t *req, int *req_length);
	int (*send_msg_pre) (uint8_t *req, int req_length);
	ssize_t (*send) (jgcx_modbus_t *ctx, const uint8_t *req, int req_length);
	int (*receive) (jgcx_modbus_t *ctx, uint8_t *req);
	ssize_t (*recv) (jgcx_modbus_t *ctx, uint8_t *rsp, int rsp_length);
	int (*check_integrity) (jgcx_modbus_t *ctx, uint8_t *msg,
	                        const int msg_length);
	int (*pre_check_confirmation) (jgcx_modbus_t *ctx, const uint8_t *req,
	                               const uint8_t *rsp, int rsp_length);
	int (*connect) (jgcx_modbus_t *ctx);
	void (*close) (jgcx_modbus_t *ctx);
	int (*flush) (jgcx_modbus_t *ctx);
	int (*select) (jgcx_modbus_t *ctx, fd_set *rset, struct timeval *tv, int msg_length);
	void (*free) (jgcx_modbus_t *ctx);
} jgcx_modbus_backend_t;

struct jgcx_modbus {
	/* Slave address */
	int slave;
	/* Socket or file descriptor */
	int s;
	int debug;
	int error_recovery;
	struct timeval response_timeout;
	struct timeval byte_timeout;
	const jgcx_modbus_backend_t *backend;
	void *backend_data;
};

void _jgcx_modbus_init_common(jgcx_modbus_t *ctx);
void _jgcx_error_print(jgcx_modbus_t *ctx, const char *context);
int _jgcx_modbus_receive_msg(jgcx_modbus_t *ctx, uint8_t *msg, jgcx_msg_type_t msg_type);

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dest, const char *src, size_t dest_size);
#endif

MODBUS_END_DECLS

#endif  /* MODBUS_PRIVATE_H */
