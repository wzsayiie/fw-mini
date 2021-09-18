#include "mformat.h"
#include <cstdio>

const size_t BufferSize = 4096;

const char *MFormatList(_Printf_format_string_ const char *format, va_list list) {
    static char *buffer = new char[BufferSize];
    vsnprintf(buffer, BufferSize, format, list);
    return buffer;
}

const char *MFormat(_Printf_format_string_ const char *format, ...) {
    const char *buffer = nullptr; {
        va_list list;
        va_start(list, format);
        buffer = MFormatList(format, list);
        va_end(list);
    }
    return buffer;
}
