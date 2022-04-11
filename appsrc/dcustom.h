#pragma once

#include "dstd.h"

namespace dash {

//lazy_var:
//

template<class Type> class lazy_var {
public:
    Type *operator->() { return  this->obj(); }
    Type *operator& () { return  this->obj(); }
    Type &operator* () { return *this->obj(); }

    void operator=(const Type &that) {
        *this->obj() = that;
    }

    operator Type() {
        return *this->obj();
    }
    
private:
    Type *obj() {
        if (!this->_obj) {
            this->_obj = new Type();
        }
        return this->_obj;
    }

    Type *_obj = nullptr;
};

template<class Type> class lazy_var<std::shared_ptr<Type>> {
public:
    Type *operator->() { return  this->obj()->get(); }
    Type *operator& () { return  this->obj()->get(); }
    Type &operator* () { return *this->obj()->get(); }

    void operator=(const std::shared_ptr<Type> &that) {
        *this->obj() = that;
    }

    operator std::shared_ptr<Type>() {
        return *this->obj();
    }

    operator Type *() {
        return this->obj()->get();
    }

    operator bool() {
        return (bool) *this->obj();
    }

private:
    std::shared_ptr<Type> *obj() {
        if (!this->_obj) {
            this->_obj = new std::shared_ptr<Type>();
        }
        return this->_obj;
    }

    std::shared_ptr<Type> *_obj = nullptr;
};

//temporary buffer:
//

void *bytes_buffer();
int   bytes_buffer_size();

template<class T> T *buffer() {
    return (T *)bytes_buffer();
}
template<class T> int buffer_size() {
    return bytes_buffer_size() / (int)sizeof(T);
}

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
