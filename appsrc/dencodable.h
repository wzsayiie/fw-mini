#pragma once

#include <type_traits>
#include "dobject.h"

namespace dash {

//field:

class d_exportable encodable_field_base {
public:
    encodable_field_base(const std::string &key);
    ~encodable_field_base();

public:
    virtual void on_encode(void *context) const;
    virtual void on_parse (void *context);
    virtual void on_clear ();
    
    const std::string &key() const;

private:
    std::string _key;
};

template<class Type, bool IsClass> class encodable_field;

template<class Type> class encodable_field<Type, true>
    : public encodable_field_base
    , public Type
{
public:
    using encodable_field_base::encodable_field_base;

public:
    void operator=(const Type &value) {
        *(Type *)this = value;
    }

    operator const Type &() const {
        return *(Type *)this;
    }
};

template<class Type> class encodable_field<Type, false>
    : public encodable_field_base
{
public:
    using encodable_field_base::encodable_field_base;

public:
    void operator=(const Type &value) {
        _value = value;
    }

    operator const Type &() const {
        return _value;
    }

private:
    Type _value {};
};

template<class Type> struct encodable
    : encodable_field<Type, std::is_class<Type>::value>
{
    typedef encodable_field<Type, std::is_class<Type>::value> _inheritable;

    using _inheritable::_inheritable;
    using _inheritable::operator=;
};

//object:

class d_exportable encodable_object
    : public    virtual_object
    , protected std::map<std::string, encodable_field_base *>
{
protected:
    static void collect(encodable_object *obj, size_t size);
};

} //end dash.

#define d_encodable_object(cls, base, body)                         \
/**/    struct cls : base {                                         \
/**/        cls() {                                                 \
/**/            dash::encodable_object::collect(this, sizeof(cls)); \
/**/        }                                                       \
/**/        struct body;                                            \
/**/    }
