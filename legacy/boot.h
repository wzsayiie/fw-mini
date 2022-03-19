#pragma once

#include <memory>

#define def_singleton(name, ...)                \
/**/    decltype(__VA_ARGS__) &name() {         \
/**/        static auto object = __VA_ARGS__;   \
/**/        return object;                      \
/**/    }

#define def_struct_and_ptr(name, ptr) typedef std::shared_ptr<struct name> ptr; struct name
#define def_class_and_ptr( name, ptr) typedef std::shared_ptr<class  name> ptr; class  name

#define def_struct(name) def_struct_and_ptr(name, name##Ref)
#define def_class( name) def_class_and_ptr (name, name##Ref)

def_class_and_ptr(base_object, base_object_ref) {
    
public:
    virtual ~base_object() = default;
    
    virtual int ref_count() const;
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
