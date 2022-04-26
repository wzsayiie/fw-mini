#include "dbuffer.h"

namespace dash {

const int c_bytes_buffer_size = 4096;

int bytes_buffer_size() {
    return c_bytes_buffer_size;
}

void *bytes_buffer() {
    static void *buffer = new char[c_bytes_buffer_size];
    return buffer;
}

} //end dash.
