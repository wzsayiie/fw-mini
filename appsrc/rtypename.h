#pragma once

#include <memory>
#include <string>
#include "dexport.h"

namespace reflect {

//type ids escape.

constexpr const char *typeids_esc = "$ESC";

//type ids:

template<class> struct typeids_of;

template<class Type> struct typeids_of<const Type *> {
    static constexpr const void *value[] = {
        "const ", typeids_esc, typeids_of<Type>::value, "*", nullptr
    };
};

template<class Type> struct typeids_of<Type *> {
    static constexpr const void *value[] = {
        typeids_esc, typeids_of<Type>::value, "*", nullptr
    };
};

template<class Type> struct typeids_of<const Type &> {
    static constexpr const void *value[] = {
        "const ", typeids_esc, typeids_of<Type>::value, "&", nullptr
    };
};

template<class Type> struct typeids_of<Type &> {
    static constexpr const void *value[] = {
        typeids_esc, typeids_of<Type>::value, "&", nullptr
    };
};

template<class Type> struct typeids_of<const std::shared_ptr<Type> &> {
    static constexpr const void *value[] = {
        "const std::shared_ptr<", typeids_esc, typeids_of<Type>::value, ">&", nullptr
    };
};

template<class Type> struct typeids_of<std::shared_ptr<Type>> {
    static constexpr const void *value[] = {
        "std::shared_ptr<", typeids_esc, typeids_of<Type>::value, ">", nullptr
    };
};

//arg types ids:

template<class> struct arg_types_ids;

template<class Type> struct arg_types_ids<void (Type)> {
    static constexpr const void *value[] = {
        typeids_esc, typeids_of<Type>::value, nullptr
    };
};

template<class First, class... Rest> struct arg_types_ids<void (First, Rest...)> {
    static constexpr const void *value[] = {
        typeids_esc, typeids_of<First>::value,
        ",",
        typeids_esc, arg_types_ids<void (Rest...)>::value,
        nullptr
    };
};

//builtin types:

template<> struct typeids_of<void       > { static constexpr const void *value[] = { "void"  , nullptr }; };
template<> struct typeids_of<bool       > { static constexpr const void *value[] = { "bool"  , nullptr }; };
template<> struct typeids_of<char       > { static constexpr const void *value[] = { "char"  , nullptr }; };
template<> struct typeids_of<uint8_t    > { static constexpr const void *value[] = { "byte"  , nullptr }; };
template<> struct typeids_of<int        > { static constexpr const void *value[] = { "int"   , nullptr }; };
template<> struct typeids_of<int64_t    > { static constexpr const void *value[] = { "int64" , nullptr }; };
template<> struct typeids_of<float      > { static constexpr const void *value[] = { "float" , nullptr }; };
template<> struct typeids_of<double     > { static constexpr const void *value[] = { "double", nullptr }; };
template<> struct typeids_of<std::string> { static constexpr const void *value[] = { "string", nullptr }; };

//type name:

d_exportable std::string make_typename(void **ids);

template<class Type> struct typename_of {
    static const char *value() {
        static std::string nm = make_typename((void **)typeids_of<Type>::value);
        return nm.c_str();
    }
};

} //end reflect.
