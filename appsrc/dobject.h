#pragma once

#include <memory>

namespace dash {

//pod:

struct pod_object {
};

//virtual:

struct virtual_object {
    virtual ~virtual_object() = default;
};

struct object : virtual_object {
    typedef std::shared_ptr<object> ptr;

    static ptr create() {
        return std::make_shared<object>();
    }
};

template<class Class, class Super> struct extends : Super {
    typedef std::shared_ptr<Class> ptr;
    
    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }
};

} //end dash.
