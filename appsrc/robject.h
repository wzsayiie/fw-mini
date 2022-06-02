#pragma once

#include "rsymbol.h"

namespace reflect {

template<> struct typeids_of<class object> {
    static constexpr const void *value[] = { "object", nullptr };
};

class dash_exportable object : public dash::extends<object, dash::object> {
public:
    static object::ptr create() {
        return object::ptr(new object, [](object *a) {
            a->release();
        });
    }
    
public:
    virtual symbol class_symbol() const;
    
    object::ptr shared();
    
    int  ref_count() const;
    void retain();
    void release();
    
private:
    int _ref_count = 1;
};

template<class Class, class Super> class dash_exportable extends : public dash::extends<Class, Super> {
public:
    template<class... Args> static std::shared_ptr<Class> create(Args... args) {
        return std::shared_ptr<Class>(new Class(args...), [](Class *a) {
            return a->release();
        });
    }

public:
    using   dash::extends<Class, Super>::extends;
    typedef extends<Class, Super> base;
    
public:
    symbol class_symbol() const override {
        return symbol_of<Class>::value();
    }
    
    std::shared_ptr<Class> shared() {
        return std::static_pointer_cast<Class>(object::shared());
    }
};

} //end reflect.
