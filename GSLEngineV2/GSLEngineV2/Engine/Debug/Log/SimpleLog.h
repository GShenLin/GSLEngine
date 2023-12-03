#pragma once

#if defined (_WIN32)
//#include "../../../simple_library/public/simple_library.h"

#define Engine_Log(format)  printf_s(format);// log_log(format , __VA_ARGS__)
#define Engine_Log_Success(format)  printf_s(format); //log_success(format , __VA_ARGS__)
#define Engine_Log_Error(format)  printf_s(format); //log_error(format , __VA_ARGS__)
#define Engine_Log_Warning(format)  printf_s(format);  // log_warning(format , __VA_ARGS__)

#elif 0

#define Engine_log(format,...) 
#define Engine_Log_Success(format,...) 
#define Engine_Log_Error(format,...)
#define Engine_Log_Warning(format,...)

#endif