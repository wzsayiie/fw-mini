#pragma once

#include "dstd.h"

namespace dash {

//lazy_var:
//

template<class Type> class lazy_var {
public:
    Type *operator->() { return  this->ptr(); }
    Type &operator* () { return *this->ptr(); }
    
    Type *ptr() {
        if (!this->_obj) {
            this->_obj = new Type();
        }
        return this->_obj;
    }

private:
    Type *_obj = nullptr;
};

//auto_assign_ptr:
//a pointer that automatically generate object.
//

template<class Type> class auto_assign_ptr {
public:
    Type *operator->() { return this->shared().get(); }
    Type &operator* () { return this->shared().get(); }

    const std::shared_ptr<Type> &shared() {
        if (!this->_ptr) {
            this->_ptr = std::make_shared<Type>();
        }
        return this->_ptr;
    }

private:
    std::shared_ptr<Type> _ptr;
};

//object:
//

struct object {
    typedef std::shared_ptr<object> ptr;
    virtual ~object() = default;
};

template<class Class, class Super> struct extends : Super {
    typedef std::shared_ptr<Class> ptr;
    
    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }
};

} //end dash.
