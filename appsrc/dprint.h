#pragma once

#include <sstream>
#include "dexport.h"

namespace dash {

d_exportable void print_string(const char *str);

template<class Last>
void print_with_stream(std::stringstream &stream, Last last) {
    stream << last;
    print_string(stream.str().c_str());
}

template<class First, class... Others>
void print_with_stream(std::stringstream &stream, First first, Others... others) {
    stream << first << " ";
    print_with_stream(stream, others...);
}

template<class... Args> void print(Args... args) {
    std::stringstream stream;
    stream << std::boolalpha;
    print_with_stream(stream, args...);
}

} //end dash.
