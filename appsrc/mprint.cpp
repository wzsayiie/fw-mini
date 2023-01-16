#include "mprint.h"
#include "dprint.h"
#include "rdefine.h"

define_reflectable_function(MPrint, "args:message")
void MPrint(const char *message) {
    dash::print(message);
}
