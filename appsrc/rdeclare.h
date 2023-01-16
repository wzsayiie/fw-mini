#pragma once

#include "rtypename.h"

//NOTE: an explicit template specialization must be declared
//in the namespace that includes the template (although msvc and clang have not this limitation).
//so 'declare_reflectable_xx' only can be used in the global scope.

#define declare_reflectable_class(Name)                                     \
/**/    class Name;                                                         \
/**/    template<> struct reflect::typeids_of<Name> {                       \
/**/        static constexpr const void *value[] = { #Name, nullptr };      \
/**/    };

#define declare_reflectable_enum(Name)                                      \
/**/    enum class Name;                                                    \
/**/    template<> struct reflect::typeids_of<Name> {                       \
/**/        static constexpr const void *value[] = { #Name, nullptr };      \
/**/    };
