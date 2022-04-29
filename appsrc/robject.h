#pragma once

#include "dash.h"
#include "rsymbol.h"

namespace reflect {

template<> struct type_name_of<class object> {
    static constexpr const char *const name = "object";
};

class object : public dash::extends<object, dash::object> {
public:
    virtual symbol *class_name() const;
    
    int  ref_count() const;
    void retain();
    void release();
    
private:
    int _ref_count = 1;
};

template<class Class, class Super> struct extends : dash::extends<Class, Super> {
    symbol *class_name() const override {
        return type_name<Class>::value();
    }
};

template<class Object> typename Object::ptr shared(Object *obj) {
    if (!obj) {
        return typename Object::ptr();
    }

    obj->retain();
    return typename Object::ptr(obj, [](Object *a) { a->release(); });
}

} //end reflect.
