#pragma once

#include <functional>
#include <memory>

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

//defer:
//

class defer {
public:
    template<class Fcn> defer(const Fcn &fcn) {
        _fcn = fcn;
    }

    ~defer() {
        _fcn();
    }

private:
    std::function<void ()> _fcn;
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

//file operations:
//

void read_file (const char *path, const std::function<void *(int size)> &buffer);
void write_file(const char *path, const void *bytes, int size);

//object:
//

struct trivial_object {
};

struct virtual_object {
    virtual ~virtual_object() = default;
};

struct object : virtual_object {
    typedef std::shared_ptr<object> ptr;

    static ptr create() {
        return std::make_shared<object>();
    }
};

template<class Class, class Super> struct extends : Super {
    typedef std::shared_ptr<Class> ptr;
    
    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }
};

} //end dash.

//any name:
//

#define  dash_name      _dash_name_a(__LINE__)
#define _dash_name_a(n) _dash_name_b(n)
#define _dash_name_b(n) _dash_name_##n

//defer macro:
//

#define dash_defer dash::defer dash_name = [&]()
