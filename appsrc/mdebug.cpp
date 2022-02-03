#include "mdebug.h"
#include <cstdio>

const size_t BufferSize = 4096;

const char * MFormatVArgs(_Printf_format_string_ const char *format, va_list list) {
    static char *buffer = new char[BufferSize];
    vsnprintf(buffer, BufferSize, format, list);
    return buffer;
}

const char *MFormat(_Printf_format_string_ const char *format, ...) {
    const char *buffer; {
        va_list list;
        va_start(list, format);
        buffer = MFormatVArgs(format, list);
        va_end(list);
    }
    return buffer;
}

#if M_OS_ANDROID
    #include <android/log.h>
    static void PrintError(const char *message) {
        __android_log_write(ANDROID_LOG_ERROR, "zzz", message);
    }
#else
    static void PrintError(const char *message) {
        fprintf(stderr, "%s\n", message);
    }
#endif

void MDebug(_Printf_format_string_ const char *format, ...) {
    const char *message; {
        va_list list;
        va_start(list, format);
        message = MFormatVArgs(format, list);
        va_end(list);
    }
    PrintError(message);
}
