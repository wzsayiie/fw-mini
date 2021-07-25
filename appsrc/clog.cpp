#include "clog.h"

const size_t MessageBufferSize = 4096;

void CLog(_Printf_format_string_ const char *format, ...) {
    char buffer[MessageBufferSize] = "\0";
    
    va_list list;
    va_start(list, format);
    vsnprintf(buffer, MessageBufferSize, format, list);
    va_end(list);
    
    MStringRef message = m_auto_release MStringCreateU8(buffer);
    MPrintMessage(message.get());
}
