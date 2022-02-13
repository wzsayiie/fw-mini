#include "robject.h"

namespace reflect {

symbol *object::static_super() {
    return nullptr;
}

symbol *object::static_class() {
    return class_name_of<object>::value();
}

symbol *object::class_name() const {
    return static_class();
}

int object::ref_count() const {
    return _ref_count;
}

object *object::retain() {
    _ref_count++;
    return this;
}

void object::release() {
    if (--_ref_count == 0) {
        delete this;
    }
}

} //end reflect.
