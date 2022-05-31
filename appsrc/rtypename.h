#pragma once

#include "dash.h"

namespace reflect {

//type ids escape.

constexpr const char *const type_ids_esc = "$ESC";

//type ids:

template<class> struct type_ids;

template<class Type> struct type_ids<const Type *> {
    static constexpr const void *ids[] = {
        "const ", type_ids_esc, type_ids<Type>::ids, "*", nullptr
    };
};

template<class Type> struct type_ids<Type *> {
    static constexpr const void *ids[] = {
        type_ids_esc, type_ids<Type>::ids, "*", nullptr
    };
};

template<class Type> struct type_ids<const Type &> {
    static constexpr const void *ids[] = {
        "const ", type_ids_esc, type_ids<Type>::ids, "&", nullptr
    };
};

template<class Type> struct type_ids<Type &> {
    static constexpr const void *ids[] = {
        type_ids_esc, type_ids<Type>::ids, "&", nullptr
    };
};

template<class Type> struct type_ids<const std::shared_ptr<Type> &> {
    static constexpr const void *ids[] = {
        "const std::shared_ptr<", type_ids_esc, type_ids<Type>::ids, ">&", nullptr
    };
};

template<class Type> struct type_ids<std::shared_ptr<Type>> {
    static constexpr const void *ids[] = {
        "std::shared_ptr<", type_ids_esc, type_ids<Type>::ids, ">", nullptr
    };
};

//arg types ids:

template<class> struct arg_types_ids;

template<class Type> struct arg_types_ids<void (Type)> {
    static constexpr const void *ids[] = {
        type_ids_esc, type_ids<Type>::ids, nullptr
    };
};

template<class First, class... Rest> struct arg_types_ids<void (First, Rest...)> {
    static constexpr const void *ids[] = {
        type_ids_esc, type_ids<First>::ids,
        ",",
        type_ids_esc, arg_types_ids<void (Rest...)>::ids,
        nullptr
    };
};

//builtin types:

template<> struct type_ids<void       > { static constexpr const void *ids[] = { "void"  , nullptr }; };
template<> struct type_ids<bool       > { static constexpr const void *ids[] = { "bool"  , nullptr }; };
template<> struct type_ids<uint8_t    > { static constexpr const void *ids[] = { "byte"  , nullptr }; };
template<> struct type_ids<int        > { static constexpr const void *ids[] = { "int"   , nullptr }; };
template<> struct type_ids<int64_t    > { static constexpr const void *ids[] = { "int64" , nullptr }; };
template<> struct type_ids<float      > { static constexpr const void *ids[] = { "float" , nullptr }; };
template<> struct type_ids<double     > { static constexpr const void *ids[] = { "double", nullptr }; };
template<> struct type_ids<std::string> { static constexpr const void *ids[] = { "string", nullptr }; };

//type name:

dash_exportable std::string make_type_name(void **ids);

template<class Type> struct type_name {
    static const char *name() {
        static std::string nm = make_type_name((void **)type_ids<Type>::ids);
        return nm.c_str();
    }
};

} //end reflect.
