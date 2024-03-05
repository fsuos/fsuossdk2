#include "SMDController.hpp"
//#include "WSLogServer.h"
#include <boost/lexical_cast.hpp>
#include <stdarg.h>
#include <iostream>
#ifndef SMDDEVICE
#include "CacheHelper.h"
#endif
void SMDController::RTLog(const char*pFormat, ...)
{
    /*va_list args;
    va_start(args, pFormat);
    WSLogServerInstance::Instance().RTLog(pFormat, args);
    va_end(args);*/
}


void SMDController::OpenLog(const std::string& file_name)
{
    //return;
#ifndef SMDDEVICE
    std::string logFile = "logs/";
    logFile += file_name;
    try {
        spdloger_ = spdlog::get(file_name);
        if(spdloger_ == nullptr) {
            //spdloger_ = spdlog::stdout_logger_mt(file_name);
            spdloger_ = spdlog::rotating_logger_mt (file_name, logFile, 1048576 * 5, 1);
            spdloger_->set_level (spdlog::level::trace);
        }
    } catch(const spdlog::spdlog_ex& ex) {
        std::cout << "OpenLog" << file_name << " Exception:" << ex.what() << std::endl;
    }
#endif
}


