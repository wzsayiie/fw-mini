#pragma once

#include "rmeta.h"

//NOTE: an explicit template specialization must be declared
//in the namespace that includes the template (although msvc and clang have not this limitation).
//so 'declare_reflectable_xx' only can be used in the global scope.

#define declare_reflectable_class(Name)                             \
/**/    class Name;                                                 \
/**/    template<> struct reflect::typeids_of<Name> {               \
/**/        static constexpr const void *value[] = {#Name, nullptr};\
/**/    };

#define declare_reflectable_enum(Name)                              \
/**/    enum class Name;                                            \
/**/    template<> struct reflect::typeids_of<Name> {               \
/**/        static constexpr const void *value[] = {#Name, nullptr};\
/**/    };

#define define_reflectable_const(Const)                             \
/**/    static reflect::committor _unused_##Const(#Const, Const);   \

#define define_reflectable_function(Function, ...)                  \
/**/    static reflect::committor _unused_##Function(               \
/**/        #Function, Function, ##__VA_ARGS__                      \
/**/    );

#define define_reflectable_class_const(Class, Const)                \
/**/    static reflect::committor _unused_##Class##_##Const(        \
/**/        reflect::symbol_of<Class>::value(),                     \
/**/        #Const,                                                 \
/**/        Class::Const                                            \
/**/    );

#define define_reflectable_class_function(Class, Function, ...)     \
/**/    static reflect::committor _unused_##Class##_##Function(     \
/**/        reflect::symbol_of<Class>::value(),                     \
/**/        #Function,                                              \
/**/        &Class::Function,                                       \
/**/        ##__VA_ARGS__                                           \
/**/    );

#define define_reflectable_enum_const(Enum, Const)                  \
/**/    static reflect::committor _unused_##Enum##_##Const(         \
/**/        0,                                                      \
/**/        reflect::symbol_of<Enum>::value(),                      \
/**/        #Const,                                                 \
/**/        (int)Enum::Const                                        \
/**/    );

#define implement_injectable_function(Ret, ...)                     \
/**/    if (auto f = find_injected(__func__)) {                     \
/**/        auto r = f->call_with_args({ this, ##__VA_ARGS__ });    \
/**/        return reflect::query<Ret>::from(r);                    \
/**/    }

namespace reflect {

//qualifier:

template<class Type> struct qualifier_of                                { static const auto value = qualifier::value       ; };
template<class Type> struct qualifier_of<const Type *                 > { static const auto value = qualifier::const_ptr   ; };
template<class Type> struct qualifier_of<Type *                       > { static const auto value = qualifier::ptr         ; };
template<class Type> struct qualifier_of<const Type &                 > { static const auto value = qualifier::const_ref   ; };
template<class Type> struct qualifier_of<Type &                       > { static const auto value = qualifier::ref         ; };
template<class Type> struct qualifier_of<const std::shared_ptr<Type> &> { static const auto value = qualifier::const_shared; };
template<class Type> struct qualifier_of<std::shared_ptr<Type>        > { static const auto value = qualifier::shared      ; };

//arguments appender:

template<class> struct arg_appender;

template<class First, class... Rest> struct arg_appender<void (First, Rest...)> {
    static void append(function_meta *meta) {
    }
};

template<> struct arg_appender<void ()> {
    static void append(function_meta *meta) {
    }
};

//committor:

class committor {
public:
    //global constant:
    committor(const char *n, std::string &v) noexcept { c_v(n, v, category::is_string); }
    committor(const char *n, double       v) noexcept { c_v(n, v, category::is_double); }
    committor(const char *n, float        v) noexcept { c_v(n, v, category::is_float ); }
    committor(const char *n, int64_t      v) noexcept { c_v(n, v, category::is_int64 ); }
    committor(const char *n, int          v) noexcept { c_v(n, v, category::is_int   ); }

    //global function.
    template<class Ret, class... Args> committor(
        const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        auto fcn_type = symbol_of<function<Ret (Args...)>>::value();
        c_fm<Ret, Args...>(fcn_type, note);

        commit_function(name, fcn_type, function<Ret (Args...)>::create([=](Args... args) {
            return fcn(args...);
        }));
    }

    //class static constant:
    committor(const symbol &c, const char *n, const std::string &v) noexcept { c_cv(c, n, v, category::is_string); }
    committor(const symbol &c, const char *n, double             v) noexcept { c_cv(c, n, v, category::is_double); }
    committor(const symbol &c, const char *n, float              v) noexcept { c_cv(c, n, v, category::is_float ); }
    committor(const symbol &c, const char *n, int64_t            v) noexcept { c_cv(c, n, v, category::is_int64 ); }
    committor(const symbol &c, const char *n, int                v) noexcept { c_cv(c, n, v, category::is_int   ); }

    //class static function.
    template<class Ret, class... Args> committor(
        const symbol &cls, const char *name, Ret (*fcn)(Args...),
        const char *note = nullptr) noexcept
    {
        commit_type_meta(cls, category::is_class);
        commit_class(cls.str(), cls);

        auto fcn_type = symbol_of<function<Ret (Args...)>>::value();
        c_fm<Ret, Args...>(fcn_type, note);

        commit_class_function(cls, name, fcn_type, function<Ret (Args...)>::create([=](Args... args) {
            return fcn(args...);
        }));
    }

    //class instance function.
    template<class Ret, class Class, class... Args> committor(
        const symbol &cls, const char *name, Ret (Class::*fcn)(Args...),
        const char *note = nullptr) noexcept
    {
        commit_type_meta(cls, category::is_class);
        commit_class(cls.str(), cls);

        auto fcn_type = symbol_of<function<Ret (const std::shared_ptr<Class> &, Args...)>>::value();
        c_fm<Ret, Args...>(fcn_type, note);

        commit_class_function(cls, name, fcn_type, function<Ret (const std::shared_ptr<Class> &, Args...)>::create(
            [=](const std::shared_ptr<Class> &self, Args... args) {
                return (self.get()->*fcn)(args...);
            }
        ));
    }

    //enumeration value.
    committor(int, const symbol &enu, const char *name, int value) noexcept {
        commit_type_meta(enu, category::is_enum);
        commit_enum(enu.str(), enu);

        auto value_type = symbol_of<int>::value();
        commit_type_meta(value_type, category::is_int);
        commit_enum_value(enu, name, value_type, value);
    }

private:
    template<class Type> void c_v(
        const char *name, const Type &value, category cate)
    {
        auto value_type = symbol_of<Type>::value();
        commit_type_meta(value_type, cate);
        commit_variable(name, value_type, value);
    }

    template<class Type> void c_cv(
        const symbol &cls, const char *name, const Type &value, category cate)
    {
        commit_type_meta(cls, category::is_class);
        commit_class(cls.str(), cls);

        auto value_type = symbol_of<Type>::value();
        commit_type_meta(value_type, cate);
        commit_class_variable(cls, name, value_type, value);
    }

    template<class Ret, class... Args> void c_fm(const symbol &sym, const char *note) {
        auto meta = (function_meta *)commit_type_meta(sym, category::is_function);
        if (!meta) {
            return;
        }
    }
};

} //end reflect.
