#pragma once

#include <sstream>
#include "denviron.h"

#if DASH_OS_ANDROID
    #include <android/log.h>
    inline void dash_puts(const char *message) {
        __android_log_write(ANDROID_LOG_INFO, "aaa", message);
    }
#else
    #include <cstdio>
    inline void dash_puts(const char *message) {
        puts(message);
    }
#endif

namespace dash {

template<class Last> void print_with_stream(std::stringstream &stream, Last last) {
    stream << last;
    dash_puts(stream.str().c_str());
}

template<class First, class... Others> void print_with_stream(std::stringstream &stream, First first, Others... others) {
    stream << first << " ";
    print_with_stream(stream, others...);
}

template<class... Args> void print(Args... args) {
    std::stringstream stream;
    print_with_stream(stream, args...);
}

} //end dash.
