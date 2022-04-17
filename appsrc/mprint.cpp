#include "mprint.h"
#include "reflect.h"

define_reflectable_function(MPrint, "args:message")
void MPrint(const std::string &message) {
    dash::print(message);
}
