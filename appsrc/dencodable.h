#pragma once

#include <type_traits>
#include "dobject.h"

namespace dash {

//member base:

class d_exportable encodable_base {
public:
    encodable_base(const std::string &key);
    virtual ~encodable_base();

public:
    const std::string key() const;

protected:
    virtual void encode(void *context) const;
    virtual void decode(void *context);

private:
    std::string _key;
};

//member implementations:

template<class Type, bool IsClass> class encodable_value;

template<class Type> class encodable_value<Type, true>
    : public encodable_base
    , public Type
{
public:
    using encodable_base::encodable_base;

public:
    void operator=(const Type &value) {
        *(Type *)this = value;
    }

    operator const Type &() const {
        return *(Type *)this;
    }
};

template<class Type> class encodable_value<Type, false>
    : public encodable_base
{
public:
    using encodable_base::encodable_base;

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
    : encodable_value<Type, std::is_class<Type>::value>
{
    typedef encodable_value<Type, std::is_class<Type>::value> _inheritable;

    using _inheritable::_inheritable;
    using _inheritable::operator=;
};

//container:

struct d_exportable encodable_object
    : virtual_object
    , std::map<std::string, encodable_base *>
{
};

d_exportable
void collect_sub_encodable(encodable_object *obj, size_t size);

} //end dash.

#define d_encodable_object(cls)                             \
/**/    cls() {                                             \
/**/        dash::collect_sub_encodable(this, sizeof(cls)); \
/**/    }
