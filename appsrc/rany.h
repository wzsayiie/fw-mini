#pragma once

#include "robject.h"

namespace reflect {

class dash_exportable any {
public:
    any();
    any(nullptr_t);
    
    any(bool               value);
    any(uint8_t            value);
    any(int                value);
    any(int64_t            value);
    any(float              value);
    any(double             value);
    any(const std::string &value);
    any(object            *value);
    any(const object::ptr &value);

    //if is a object ptr.
    template<class Type> any(Type *value)
        : any((object *)value) {}

    //if is a object shared ptr.
    template<class Type> any(const std::shared_ptr<Type> &value)
        : any(std::static_pointer_cast<object>(value)) {}

    //if is a integer constants.
    template<class Type> any(Type value)
        : any((int)value) {}
    
    bool               as_bool        () const;
    uint8_t            as_byte        () const;
    int                as_int         () const;
    int64_t            as_int64       () const;
    float              as_float       () const;
    double             as_double      () const;
    const std::string &as_const_string() const;
    std::string        as_string      () const;
    const object      *as_const_ptr   () const;
    object            *as_ptr         () const;
    const object::ptr &as_const_shared() const;
    object::ptr        as_shared      () const;

    operator bool               () const;
    operator uint8_t            () const;
    operator int                () const;
    operator int64_t            () const;
    operator float              () const;
    operator double             () const;
    operator const std::string &() const;
    operator std::string        () const;
    operator const object      *() const;
    operator object            *() const;
    operator const object::ptr &() const;
    operator object::ptr        () const;

    //if a object ptr.
    template<class Type> operator const Type *() const {
        return (const Type *)as_const_ptr();
    }
    template<class Type> operator Type *() const {
        return (Type *)as_ptr();
    }

    //if a object shared ptr.
    template<class Type> operator const std::shared_ptr<Type> &() const {
        return std::dynamic_pointer_cast<Type>(as_const_shared());
    }
    template<class Type> operator std::shared_ptr<Type>() const {
        return std::dynamic_pointer_cast<Type>(as_shared());
    }

    //if a integer constants.
    template<class Type> operator Type() const {
        return (Type)as_int();
    }
    
private:
    enum class data_type {
        is_void  ,
        is_bool  ,
        is_byte  , is_int   , is_int64 ,
        is_float , is_double,
        is_string, is_object,
    };
    union data_word {
        int64_t as_void  ;
        bool    as_bool  ;
        uint8_t as_byte  ;
        int     as_int   ;
        int64_t as_int64 ;
        float   as_float ;
        double  as_double;
    };
    
    data_type _type = data_type::is_void;
    data_word _word = {0};
    std::string _string;
    object::ptr _object;
};

} //end reflect.
