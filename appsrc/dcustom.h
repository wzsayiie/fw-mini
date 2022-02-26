#pragma once

#include "dstd.h"

namespace dash {

template<class Type> struct lazy_var {
    Type *operator->() const { return &value(); }
    Type &operator* () const { return  value(); }
    
    Type &value() const {
        static Type obj;
        return obj;
    }
};

struct object {
    typedef std::shared_ptr<object> ptr;
    virtual ~object() = default;
};

template<class Class, class Super> struct extends : Super {
    typedef std::shared_ptr<Class> ptr;
    
    static ptr create() {
        return std::make_shared<Class>();
    }
};

} //end dash.
