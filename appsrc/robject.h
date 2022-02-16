#pragma once

#include "rutility.h"

namespace reflect {

template<class> struct class_name_of {
    static symbol *value() { return nullptr; }
};

template<class Class, class Super> struct extends : dash::extends<Class, Super> {
    static symbol *static_super() { return class_name_of<Super>::value(); }
    static symbol *static_class() { return class_name_of<Class>::value(); }
    
    symbol *class_name() const override {
        return static_class();
    }
};

template<> struct class_name_of<class object> {
    static symbol *value() {
        static symbol *sym = symbol::make("object");
        return sym;
    }
};

class object : public dash::extends<object, dash::object> {
public:
    static symbol *static_super();
    static symbol *static_class();
    
    virtual symbol *class_name() const;
    
    virtual int ref_count() const;
    virtual object *retain();
    virtual void release();
    
private:
    int _ref_count = 1;
};

} //end reflect.
