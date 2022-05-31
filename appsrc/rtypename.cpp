#include "rtypename.h"
#include <cctype>
#include <sstream>

namespace reflect {

static bool is_string(char *str) {
    if (!isprint(str[0])) {
        return false;
    }
    for (int i = 1; i < 8 && str[i]; ++i) {
        if (!isprint(str[i])) {
            return false;
        }
    }

    return true;
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
