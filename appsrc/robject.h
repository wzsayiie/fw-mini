#pragma once

#include "dobject.h"
#include "rsymbol.h"

namespace reflect {

//object:

template<> struct typeids_of<class object> {
    static constexpr const void *value[] = { "object", nullptr };
};

class d_exportable object : public dash::extends<object, dash::object> {
public:
    virtual symbol class_symbol() const {
        return symbol_of<object>::value();
    }
};

//extends:

template<class Class, class Base> class d_exportable extends : public dash::extends<Class, Base> {
public:
    typedef extends<Class, Base> super;

public:
    using _middle = dash::extends<Class, Base>;
    using _middle::_middle;
    
public:
    symbol class_symbol() const override {
        return symbol_of<Class>::value();
    }
};

//cast:

template<class Type> struct pointer_cast_as {
    static std::shared_ptr<Type> from(const object::ptr &obj) {
        return std::dynamic_pointer_cast<Type>(obj);
    }
};

} //end reflect.
