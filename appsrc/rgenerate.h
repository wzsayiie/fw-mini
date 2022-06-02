#pragma once

#include "rmeta.h"

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

#define define_reflectable_const(Const)                                     \
/**/    static reflect::committor _unused_##Const(#Const, Const);           \

#define define_reflectable_function(Function, ...)                          \
/**/    static reflect::committor _unused_##Function(                       \
/**/        #Function, Function, ##__VA_ARGS__                              \
/**/    );

#define define_reflectable_class_const(Class, Const)                        \
/**/    static reflect::committor _unused_##Class##_##Const(                \
/**/        (Class *)nullptr, #Const, Class::Const                          \
/**/    );

#define define_reflectable_class_function(Class, Function, ...)             \
/**/    static reflect::committor _unused_##Class##_##Function(             \
/**/        (Class *)nullptr, #Function, &Class::Function, ##__VA_ARGS__    \
/**/    );

#define define_reflectable_enum_const(Enum, Const)                          \
/**/    static reflect::committor _unused_##Enum##_##Const(                 \
/**/        (Enum)0, #Const, (int)Enum::Const                               \
/**/    );

#define implement_injectable_function(Ret, ...)                             \
/**/    if (auto f = find_injected(__func__)) {                             \
/**/        auto r = f->call_with_args({ this, ##__VA_ARGS__ });            \
/**/        return reflect::take<Ret>::from(r);                             \
/**/    }

namespace reflect {

//qualifier:

template<class T> struct qualifier_of                             { static constexpr const auto value = qualifier::value       ; };
template<class T> struct qualifier_of<const T *                 > { static constexpr const auto value = qualifier::const_ptr   ; };
template<class T> struct qualifier_of<T *                       > { static constexpr const auto value = qualifier::ptr         ; };
template<class T> struct qualifier_of<const T &                 > { static constexpr const auto value = qualifier::const_ref   ; };
template<class T> struct qualifier_of<T &                       > { static constexpr const auto value = qualifier::ref         ; };
template<class T> struct qualifier_of<const std::shared_ptr<T> &> { static constexpr const auto value = qualifier::const_shared; };
template<class T> struct qualifier_of<std::shared_ptr<T>        > { static constexpr const auto value = qualifier::shared      ; };

//arguments appender:

template<class> struct arg_appender;

template<class First, class... Rest> struct arg_appender<void (First, Rest...)> {
    static void append(function_meta *meta) {
        meta->arg_quals.push_back(qualifier_of<First>::value);
        meta->arg_types.push_back(symbol_of<First>::value());

        arg_appender<void (Rest...)>::append(meta);
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
        //type.
        auto type = c_fm<Ret, Args...>(note);

        //value.
        commit_function(name, type, function<Ret (Args...)>::create([=](Args... args) {
            return fcn(args...);
        }));
    }

    //class static constant:
    template<class C> committor(C *c, const char *n, const std::string &v) noexcept { c_cv(c, n, v, category::is_string); }
    template<class C> committor(C *c, const char *n, double             v) noexcept { c_cv(c, n, v, category::is_double); }
    template<class C> committor(C *c, const char *n, float              v) noexcept { c_cv(c, n, v, category::is_float ); }
    template<class C> committor(C *c, const char *n, int64_t            v) noexcept { c_cv(c, n, v, category::is_int64 ); }
    template<class C> committor(C *c, const char *n, int                v) noexcept { c_cv(c, n, v, category::is_int   ); }

    //class static function.
    template<class Ret, class Class, class... Args> committor(
        Class *, const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        //class type.
        symbol cls_type = c_cm<Class>();

        //class value.
        commit_class(cls_type.str(), cls_type);

        //function type.
        auto fcn_type = c_fm<Ret, Args...>(note);

        //function value.
        commit_class_function(cls_type, name, fcn_type, function<Ret (Args...)>::create([=](Args... args) {
            return fcn(args...);
        }));
    }

    //class instance function.
    template<class Ret, class Class, class... Args> committor(
        Class *, const char *name, Ret (Class::*fcn)(Args...), const char *note = nullptr) noexcept
    {
        //class type.
        symbol cls_type = c_cm<Class>();

        //class value.
        commit_class(cls_type.str(), cls_type);

        //function type.
        auto fcn_type = c_fm<Ret, Args...>(note);

        //function value.
        commit_object_function(cls_type, name, fcn_type, function<Ret (const std::shared_ptr<Class> &, Args...)>::create(
            [=](const std::shared_ptr<Class> &self, Args... args) {
                return (self.get()->*fcn)(args...);
            }
        ));
    }

    //enumeration value.
    template<class Enum> committor(Enum, const char *name, int value) noexcept {
        //enum type.
        symbol enum_type = symbol_of<Enum>::value();
        commit_type_meta(enum_type, category::is_enum);

        //enum value.
        commit_enum(enum_type.str(), enum_type);

        //member type.
        auto value_type = symbol_of<int>::value();
        commit_type_meta(value_type, category::is_int);

        //member value.
        commit_enum_value(enum_type, name, value_type, value);
    }

private:
    template<class Type> void c_v(const char *name, const Type &value, category cate) {
        //type.
        auto type = symbol_of<Type>::value();
        commit_type_meta(type, cate);

        //value.
        commit_variable(name, type, value);
    }

    template<class Class, class Type> void c_cv(Class *, const char *name, const Type &value, category cate) {
        //class type.
        symbol cls_type = c_cm<Class>();

        //class value.
        commit_class(cls_type.str(), cls_type);

        //member type.
        auto value_type = symbol_of<Type>::value();
        commit_type_meta(value_type, cate);

        //member value.
        commit_class_variable(cls_type, name, value_type, value);
    }
    
    template<class Class> symbol c_cm() {
        auto cls_type = symbol_of<Class>::value();
        auto cls_meta = (class_meta *)commit_type_meta(cls_type, category::is_class);
        if (!cls_meta) {
            return cls_type;
        }
        
        //IMPORTANT: add constructor function.
        auto constructor_type = c_fm<object::ptr>(nullptr);
        commit_class_function(cls_type, "create", constructor_type, function<object::ptr ()>::create([=]() {
            return Class::create();
        }));
        
        return cls_type;
    }

    template<class Ret, class... Args> symbol c_fm(const char *note) {
        auto type = symbol_of<function<Ret (Args...)>>::value();
        auto meta = (function_meta *)commit_type_meta(type, category::is_function);
        if (!meta) {
            return type;
        }

        //arguments.
        arg_appender<void (Args...)>::append(meta);

        //returning.
        meta->ret_qual = qualifier_of<Ret>::value;
        meta->ret_type = symbol_of<Ret>::value();
        
        //note.
        meta->note = note ? note : "";
        
        return type;
    }
};

} //end reflect.
