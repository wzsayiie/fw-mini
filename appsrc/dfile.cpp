#include "dfile.h"
#include <fstream>

namespace dash {

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
