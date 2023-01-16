#include "rtypename.h"
#include <cstring>
#include <sstream>

namespace reflect {

static void concatenate(std::stringstream &stream, void **ids) {
    bool escaped = false;
    for (void **ptr = ids; *ptr; ++ptr) {
        if (strcmp((char *)*ptr, typeids_esc) == 0) {
            escaped = true;

        } else if (escaped) {
            concatenate(stream, (void **)*ptr);
            escaped = false;

        } else {
            stream << (char *)*ptr;
        }
    }
}

std::string make_typename(void **ids) {
    std::stringstream stream;
    concatenate(stream, ids);
    return stream.str();
}

} //end reflect.
