#pragma once

#include <type_traits>
#include "robject.h"

namespace reflect {

//any:

enum class data_type {
    is_void  ,
    is_bool  ,
    is_byte  , is_int   , is_int64,
    is_float , is_double,
    is_string,
    is_object,
};

template<> struct typeids_of<class any> {
    static constexpr const void *value[] = { "any", nullptr };
};

class d_exportable any {
public:
    any();
    any(nullptr_t);
    
    any(bool               value);
    any(uint8_t            value);
    any(int                value);
    any(int64_t            value);
    any(float              value);
    any(double             value);
    any(const char        *value);
    any(const std::string &value);
    any(object            *value);
    any(const object::ptr &value);

    //if is a object ptr.
    template<class Type> any(Type *value): any((object *)value) {
        static_assert(std::is_base_of<object, Type>::value);
    }

    //if is a object shared ptr.
    template<class Type> any(const std::shared_ptr<Type> &value)
        : any(std::static_pointer_cast<object>(value))
    {
        static_assert(std::is_base_of<object, Type>::value);
    }

    //if is a enum value.
    template<class Type> any(Type value): any((int)value) {
        static_assert(std::is_enum<Type>::value);
    }
    
    data_type   preferred_type  () const;
    bool        is_numeric_type () const;
    bool        as_bool         () const;
    uint8_t     as_byte         () const;
    int         as_int          () const;
    int64_t     as_int64        () const;
    float       as_float        () const;
    double      as_double       () const;
    const char *as_chars        () const;
    std::string as_string       () const;
    object *    as_object_ptr   () const;
    object::ptr as_object_shared() const;

    operator bool        () const;
    operator uint8_t     () const;
    operator int         () const;
    operator int64_t     () const;
    operator float       () const;
    operator double      () const;
    operator const char *() const;
    operator std::string () const;
    operator object *    () const;
    operator object::ptr () const;

    //if a object ptr.
    template<class Type> operator Type *() const {
        static_assert(std::is_base_of<object, Type>::value);
        return (Type *)as_object_ptr();
    }

    //if a object shared ptr.
    template<class Type> operator std::shared_ptr<Type>() const {
        static_assert(std::is_base_of<object, Type>::value);
        return pointer_cast_as<Type>::from(as_object_shared());
    }

    //if a enum value.
    template<class Type> operator Type() const {
        static_assert(std::is_enum<Type>::value);
        return (Type)as_int();
    }

    int compare(const any &that) const;

    bool operator< (const any &that) const;
    bool operator<=(const any &that) const;
    bool operator> (const any &that) const;
    bool operator>=(const any &that) const;
    bool operator==(const any &that) const;
    bool operator!=(const any &that) const;
    
private:
    union data_word {
        bool    as_bool  ;
        uint8_t as_byte  ;
        int     as_int   ;
        int64_t as_int64 ;
        float   as_float ;
        double  as_double;
    };
    
private:
    data_type _type = data_type::is_void;
    data_word _word = {0};
    std::string _string;
    object::ptr _object;
};

//query:
//NOTE: for template instantiation, specify an explicit conversion target type avoids ambiguity.

//if is a enum value.
template<class Type> struct take {
    static Type from(const any &a) {
        static_assert(std::is_enum<Type>::value);
        return (Type)a.as_int();
    }
};

//if is a any.
template<> struct take<any> {
    static any from(const any &a) {
        return a;
    }
};
template<> struct take<const any &> {
    static any from(const any &a) {
        return a;
    }
};

//if is a object ptr.
template<class Type> struct take<Type *> {
    static Type *from(const any &a) {
        static_assert(std::is_base_of<object, Type>::value);
        return (Type *)a.as_object_ptr();
    }
};

//if is a object shared ptr.
template<class Type> struct take<std::shared_ptr<Type>> {
    static std::shared_ptr<Type> from(const any &a) {
        static_assert(std::is_base_of<object, Type>::value);
        return pointer_cast_as<Type>::from(a.as_object_shared());
    }
};
template<class Type> struct take<const std::shared_ptr<Type> &> {
    static std::shared_ptr<Type> from(const any &a) {
        return take<std::shared_ptr<Type>>::from(a);
    }
};

//if is a string.
template<> struct take<const char *> {
    static const char *from(const any &a) {
        return a.as_chars();
    }
};
template<> struct take<std::string> {
    static std::string from(const any &a) {
        return a.as_string();
    }
};
template<> struct take<const std::string &> {
    static std::string from(const any &a) {
        return take<std::string>::from(a);
    }
};

template<> struct take<void   > { static void    from(const any &a) { } };
template<> struct take<bool   > { static bool    from(const any &a) { return a.as_bool  (); } };
template<> struct take<uint8_t> { static uint8_t from(const any &a) { return a.as_byte  (); } };
template<> struct take<int    > { static int     from(const any &a) { return a.as_int   (); } };
template<> struct take<int64_t> { static int64_t from(const any &a) { return a.as_int64 (); } };
template<> struct take<float  > { static float   from(const any &a) { return a.as_float (); } };
template<> struct take<double > { static double  from(const any &a) { return a.as_double(); } };

} //end reflect.
