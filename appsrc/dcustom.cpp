#include "dcustom.h"
#include <fstream>

namespace dash {

//temporary buffer:
//

const int c_bytes_buffer_size = 4096;

void *bytes_buffer() {
    static thread_local void *buffer = new uint8_t[c_bytes_buffer_size];
    return buffer;
}

int bytes_buffer_size() {
    return c_bytes_buffer_size;
}


//file operations:
//

void read_file(const char *path, const std::function<void *(int size)> &buffer) {
    if (!path || !*path || !buffer) {
        return;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    int size = 0; {
        file.seekg(0, std::ios::end);
        size = (int)file.tellg();
        file.seekg(0, std::ios::beg);
    }

    auto bytes = (char *)buffer(size);
    file.read(bytes, size);
}

void write_file(const char *path, const void *bytes, int size) {
    if (!path || !*path) {
        return;
    }

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    if (bytes && size > 0) {
        file.write((const char *)bytes, size);
    } else {
        //clear the file.
        file.write("", 0);
    }
}

} //end dash.
