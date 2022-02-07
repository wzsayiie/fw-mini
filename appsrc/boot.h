#pragma once

#include <memory>

#define def_singleton(name, ...)                \
/**/    decltype(__VA_ARGS__) &name() {         \
/**/        static auto object = __VA_ARGS__;   \
/**/        return object;                      \
/**/    }

#define _default_name(_, name, ...) name

#define def_struct(name, ...)                                                       \
/**/    struct name;                                                                \
/**/    typedef std::shared_ptr<name> _default_name(_, ##__VA_ARGS__, name##Ref);   \
/**/    struct name

#define def_class(name, ...)                                                        \
/**/    class name;                                                                 \
/**/    typedef std::shared_ptr<name> _default_name(_, ##__VA_ARGS__, name##Ref);   \
/**/    class name

def_class(base_object, base_object_ref) {
    
public:
    virtual ~base_object() = default;
    
    virtual base_object *retain();
    virtual void release();
    
private:
    int _ref_count = 1;
};

base_object *retain(base_object *object);
void release(base_object *object);

struct _cast_shared_helper {
    template<typename T> std::shared_ptr<T> operator<<(T *object) {
        retain(object);
        return std::shared_ptr<T>(object, release);
    }
};
#define cast_shared _cast_shared_helper()<<

struct _auto_release_helper {
    template<typename T> std::shared_ptr<T> operator<<(T *object) {
        return std::shared_ptr<T>(object, release);
    }
};
#define auto_release _auto_release_helper()<<
