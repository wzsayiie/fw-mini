#include "clog.h"

void CLog(_Printf_format_string_ const char *format, ...) {
    const char *chars; {
        va_list list;
        va_start(list, format);
        chars = MFormatVArgs(format, list);
        va_end(list);
    }
    
    MStringRef string = m_auto_release MStringCreateU8(chars);
    MPrintMessage(string.get());
}
