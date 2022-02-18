#pragma once

#include "dash.h"

namespace reflect {

class symbol {
public:
    static symbol *make(const char *str);
    
    bool operator<(const symbol &that) const;
    const char *str() const;
    
private:
    symbol(const char *str);
    
    const char *_str;
};

class any {
public:
    any();
    any(nullptr_t);
    
    any(bool    value);
    any(int     value);
    any(int64_t value);
    any(float   value);
    any(double  value);
    
    any(const std::string       &value);
    any(const dash::object::ptr &value);
    
    bool    as_bool  () const;
    int     as_int   () const;
    int64_t as_int64 () const;
    float   as_float () const;
    double  as_double() const;
    
    const std::string       &as_string() const;
    const dash::object::ptr &as_object() const;

    operator bool   () const;
    operator int    () const;
    operator int64_t() const;
    operator float  () const;
    operator double () const;

    operator const std::string       &() const;
    operator const dash::object::ptr &() const;

    template<class Type> operator std::shared_ptr<Type>() const {
        return std::dynamic_pointer_cast<Type>(_object);
    }
    
private:
    enum class data_type {
        is_void  ,
        is_bool  , is_int   , is_int64, is_float, is_double,
        is_string, is_object,
    };
    union data_word {
        int64_t as_void  ;
        bool    as_bool  ;
        int     as_int   ;
        int64_t as_int64 ;
        float   as_float ;
        double  as_double;
    };
    
    data_type _type = data_type::is_void;
    data_word _word = {0};
    dash::object::ptr _object;
    std::string _string;
};

} //end reflect.
