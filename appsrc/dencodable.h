#pragma once

#include <type_traits>
#include "dobject.h"

namespace dash {

//field:

class d_exportable encodable_field {
public:
    encodable_field(const std::string &key);
    ~encodable_field();

public:
    virtual void on_encode(void *context) const;
    virtual void on_parse (void *context);
    virtual void on_clear ();
    
    const std::string &key() const;

private:
    std::string _key;
};

template<class Type, bool IsClass> class encodable_field_impl;

template<class Type> class encodable_field_impl<Type, true>
    : public encodable_field
    , public Type
{
public:
    using encodable_field::encodable_field;

public:
    void operator=(const Type &value) {
        *(Type *)this = value;
    }
};

template<class Type> class encodable_field_impl<Type, false>
    : public encodable_field
{
public:
    using encodable_field::encodable_field;

public:
    void operator=(const Type &value) {
        _value = value;
    }

    operator Type() const {
        return _value;
    }

private:
    Type _value {};
};

template<class Type> struct encodable
    : encodable_field_impl<Type, std::is_class<Type>::value>
{
    typedef encodable_field_impl<Type, std::is_class<Type>::value> _inheritable;

    using _inheritable::_inheritable;
    using _inheritable::operator=;
};

//object:

class d_exportable encodable_object
    : public virtual_object
    , public std::map<std::string, encodable_field *>
{
protected:
    static void collect(encodable_object *obj, size_t size);
};

} //end dash.

#define d_encodable_object(cls, base, ...)                          \
/**/    struct cls : base {                                         \
/**/        cls() {                                                 \
/**/            dash::encodable_object::collect(this, sizeof(cls)); \
/**/        }                                                       \
/**/        struct __VA_ARGS__;                                     \
/**/    }
