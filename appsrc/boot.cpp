#include "boot.h"

base_object *base_object::retain() {
    _ref_count++;
    return this;
}

void base_object::release() {
    if (--_ref_count == 0) {
        delete this;
    }
}

base_object *retain(base_object *object) {
    if (object) {
        object->retain();
    }
    return object;
}

void release(base_object *object) {
    if (object) {
        object->release();
    }
}
