/*
 * Copyright © 2001-2011 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef JGCX_MODBUS_RTU_H
#define JGCX_MODBUS_RTU_H

#include "jgcx_modbus.h"

MODBUS_BEGIN_DECLS


#define JGCX_MODBUS_RTU_MAX_ADU_LENGTH  256

MODBUS_API jgcx_modbus_t* jgcx_modbus_new_rtu();


MODBUS_END_DECLS


#endif /* MODBUS_RTU_H */
