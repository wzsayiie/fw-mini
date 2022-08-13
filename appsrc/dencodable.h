#pragma once

#include <type_traits>
#include "dobject.h"

namespace dash {

//field:

class d_exportable encodable_field_base : virtual_object {
public:
    encodable_field_base(const std::string &key);
    ~encodable_field_base();

public:
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

struct d_exportable encodable_object
    : virtual_object
    , std::map<std::string, encodable_field_base *>
{
};

d_exportable
void collect_sub_encodable(encodable_object *obj, size_t size);

} //end dash.

#define d_encodable_object(cls)                             \
/**/    cls() {                                             \
/**/        dash::collect_sub_encodable(this, sizeof(cls)); \
/**/    }
