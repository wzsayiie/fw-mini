#include "mdebug.h"
#include <cstdarg>
#include <cstdio>

#if MOS_ANDROID
    #include <android/log.h>
    static void PrintError(const char *message) {
        __android_log_write(ANDROID_LOG_ERROR, "zzz", message);
    }
#else
    static void PrintError(const char *message) {
        fprintf(stderr, "%s\n", message);
    }
#endif

const size_t BUFFER_SIZE = 4096;

void MDebug(_Printf_format_string_ const char *format, ...){
    char buffer[BUFFER_SIZE] = "\0";
    
    va_list list;
    va_start(list, format);
    vsnprintf(buffer, BUFFER_SIZE, format, list);
    va_end(list);
    
    PrintError(buffer);
}
