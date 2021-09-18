#include "mdebug.h"
#include <cstdio>

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
    const char *message = nullptr; {
        va_list list;
        va_start(list, format);
        message = MFormatList(format, list);
        va_end(list);
    }
    PrintError(message);
}
