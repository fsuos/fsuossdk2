#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

#include "tele_c_interface.h"
#include <boost/date_time/posix_time/ptime.hpp>

std::string exec(const char* cmd);

uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);

tele_c_time get_ttime(boost::posix_time::ptime ptime);

#endif
