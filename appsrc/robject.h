#pragma once

#include "rtypes.h"

namespace reflect {

reflect_reflectable_class(object);
class object : public dash::extends<object, dash::object> {
public:
    virtual symbol *class_name() const;
    
    virtual int ref_count() const;
    virtual object *retain();
    virtual void release();
    
private:
    int _ref_count = 1;
};

template<class Class, class Super> struct extends : dash::extends<Class, Super> {
    symbol *class_name() const override {
        return type_name<Class>::value();
    }
};

} //end reflect.
