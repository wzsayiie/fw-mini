#include "robject.h"

namespace reflect {

symbol *object::class_name() const {
    return type_name<object>::value();
}

int object::ref_count() const {
    return _ref_count;
}

void object::retain() {
    _ref_count++;
}

void object::release() {
    if (--_ref_count == 0) {
        delete this;
    }
}

} //end reflect.
