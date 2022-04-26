#pragma once

#include <memory>

namespace dash {

template<class Type> class lazy {
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

template<class Type> class lazy<std::shared_ptr<Type>> {
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
        return (bool)*this->obj();
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

} //end dash.
