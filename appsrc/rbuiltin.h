#pragma once

#include "rgenerate.h"
#include "rmap.h"
#include "rset.h"
#include "rvector.h"

namespace reflect {

template<> struct type_name_of<void       > { static constexpr const char *const name = "void"  ; };
template<> struct type_name_of<bool       > { static constexpr const char *const name = "bool"  ; };
template<> struct type_name_of<uint8_t    > { static constexpr const char *const name = "byte"  ; };
template<> struct type_name_of<int        > { static constexpr const char *const name = "int"   ; };
template<> struct type_name_of<int64_t    > { static constexpr const char *const name = "int64" ; };
template<> struct type_name_of<float      > { static constexpr const char *const name = "float" ; };
template<> struct type_name_of<double     > { static constexpr const char *const name = "double"; };
template<> struct type_name_of<std::string> { static constexpr const char *const name = "string"; };

} //end reflect.

declare_reflectable_special(reflect::vector<bool>       )
declare_reflectable_special(reflect::vector<uint8_t>    )
declare_reflectable_special(reflect::vector<int>        )
declare_reflectable_special(reflect::vector<int64_t>    )
declare_reflectable_special(reflect::vector<float>      )
declare_reflectable_special(reflect::vector<double>     )
declare_reflectable_special(reflect::vector<std::string>)

declare_reflectable_special(reflect::map<std::string, bool>       )
declare_reflectable_special(reflect::map<std::string, int>        )
declare_reflectable_special(reflect::map<std::string, int64_t>    )
declare_reflectable_special(reflect::map<std::string, float>      )
declare_reflectable_special(reflect::map<std::string, double>     )
declare_reflectable_special(reflect::map<std::string, std::string>)
declare_reflectable_special(reflect::map<int        , bool>       )
declare_reflectable_special(reflect::map<int        , int>        )
declare_reflectable_special(reflect::map<int        , int64_t>    )
declare_reflectable_special(reflect::map<int        , float>      )
declare_reflectable_special(reflect::map<int        , double>     )
declare_reflectable_special(reflect::map<int        , std::string>)

declare_reflectable_special(reflect::set<bool>       )
declare_reflectable_special(reflect::set<int>        )
declare_reflectable_special(reflect::set<int64_t>    )
declare_reflectable_special(reflect::set<float>      )
declare_reflectable_special(reflect::set<double>     )
declare_reflectable_special(reflect::set<std::string>)

declare_reflectable_special(reflect::function<void ()>)
