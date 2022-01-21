#pragma once

#include <memory>

#define def_singleton(name, ...)                \
/**/    decltype(__VA_ARGS__) &name() {         \
/**/        static auto object = __VA_ARGS__;   \
/**/        return object;                      \
/**/    }

#define def_class(name) \
/**/    typedef std::shared_ptr<class name> name##Ref; \
/**/    class name

class base_object {
public:
    base_object();
    virtual ~base_object();
};
