#pragma once

#include <cstdarg>
#include "menviron.h"

#if M_CPL_CL
    #include <sal.h>
    #define __printflike(format, list)
#else
    #include <sys/cdefs.h>
    #define _Printf_format_string_
#endif

const char *MFormatList(_Printf_format_string_ const char *format, va_list list) __printflike(1, 0);

const char *MFormat(_Printf_format_string_ const char *format, ...) __printflike(1, 2);
