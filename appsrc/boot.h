#pragma once

#include <memory>

#define def_singleton(name, ...)                \
/**/    __VA_ARGS__ &name() {                   \
/**/        static auto a = new __VA_ARGS__;    \
/**/        return *a;                          \
/**/    }

#define def_class(name) \
/**/    typedef std::shared_ptr<class name> name##Ref; \
/**/    class name

class base_object {
public:
    base_object();
    virtual ~base_object();
};
