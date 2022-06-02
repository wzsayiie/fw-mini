#pragma once

#include "robject.h"

namespace reflect {

//any:

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
    
    bool        as_bool         () const;
    uint8_t     as_byte         () const;
    int         as_int          () const;
    int64_t     as_int64        () const;
    float       as_float        () const;
    double      as_double       () const;
    std::string as_string       () const;
    object *    as_object_ptr   () const;
    object::ptr as_object_shared() const;

    operator bool       () const;
    operator uint8_t    () const;
    operator int        () const;
    operator int64_t    () const;
    operator float      () const;
    operator double     () const;
    operator std::string() const;
    operator object *   () const;
    operator object::ptr() const;

    //if a object ptr.
    template<class Type> operator Type *() const {
        return (Type *)as_object_ptr();
    }

    //if a object shared ptr.
    template<class Type> operator std::shared_ptr<Type>() const {
        return std::dynamic_pointer_cast<Type>(as_object_shared());
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

//query:
//NOTE: for template instantiation, specify an explicit conversion target type avoids ambiguity.

//if is a integer constant.
template<class Type> struct query {
    static Type from(const any &a) {
        return (Type)a.as_int();
    }
};

//if is a object ptr.
template<class Type> struct query<Type *> {
    static Type *from(const any &a) {
        return (Type *)a.as_object_ptr();
    }
};

//if is a object shared ptr.
template<class Type> struct query<const std::shared_ptr<Type> &> {
    static std::shared_ptr<Type> from(const any &a) {
        return std::dynamic_pointer_cast<Type>(a.as_object_shared());
    }
};
template<class Type> struct query<std::shared_ptr<Type>> {
    static std::shared_ptr<Type> from(const any &a) {
        return std::dynamic_pointer_cast<Type>(a.as_object_shared());
    }
};

//if is a string.
template<> struct query<const std::string &> {
    static std::string from(const any &a) {
        return a.as_string();
    }
};
template<> struct query<std::string> {
    static std::string from(const any &a) {
        return a.as_string();
    }
};

template<> struct query<void   > { static void    from(const any &a) { } };
template<> struct query<bool   > { static bool    from(const any &a) { return a.as_bool  (); } };
template<> struct query<uint8_t> { static uint8_t from(const any &a) { return a.as_byte  (); } };
template<> struct query<int    > { static int     from(const any &a) { return a.as_int   (); } };
template<> struct query<int64_t> { static int64_t from(const any &a) { return a.as_int64 (); } };
template<> struct query<float  > { static float   from(const any &a) { return a.as_float (); } };
template<> struct query<double > { static double  from(const any &a) { return a.as_double(); } };

} //end reflect.
