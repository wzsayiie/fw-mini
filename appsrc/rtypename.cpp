#include "rtypename.h"
#include <cctype>
#include <sstream>

namespace reflect {

static bool is_string(char *ptr) {
    return isprint(ptr[0]) && (isprint(ptr[1]) || !ptr[1]);
}

static void concatenate(std::stringstream &stream, char **ids) {
    for (char **ptr = ids; *ptr; ++ptr) {
        if (!is_string(*ptr)) {
            concatenate(stream, (char **)*ptr);
        } else {
            stream << *ptr;
        }
    }
}

std::string make_type_name(char **ids) {
    std::stringstream stream;
    concatenate(stream, ids);
    return stream.str();
}

} //end reflect.
