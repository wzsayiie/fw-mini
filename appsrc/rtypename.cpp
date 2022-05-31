#include "rtypename.h"
#include <sstream>
#include <cstring>

namespace reflect {

static void concatenate(std::stringstream &stream, void **ids) {
    bool escaped = false;
    for (void **ptr = ids; *ptr; ++ptr) {
        if (strcmp((char *)*ptr, type_ids_esc) == 0) {
            escaped = true;

        } else if (escaped) {
            concatenate(stream, (void **)*ptr);
            escaped = false;

        } else {
            stream << (char *)*ptr;
        }
    }
}

std::string make_type_name(void **ids) {
    std::stringstream stream;
    concatenate(stream, ids);
    return stream.str();
}

} //end reflect.
