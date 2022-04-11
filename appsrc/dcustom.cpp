#pragma once

#include "dcustom.h"

namespace dash {

//temporary buffer:
//

const int c_bytes_buffer_size = 1024;

void *bytes_buffer() {
    static thread_local void *buffer = new uint8_t[c_bytes_buffer_size];
    return buffer;
}

int bytes_buffer_size() {
    return c_bytes_buffer_size;
}

} //end dash.
