#pragma once

#include <memory>

namespace dash {

//pod:

struct pod_object {
};

//virtual:

class dash_exportable virtual_object {
public:
    virtual ~virtual_object() = default;
};

class dash_exportable object : public virtual_object {
public:
    typedef std::shared_ptr<object> ptr;

public:
    static ptr create() {
        return std::make_shared<object>();
    }
};

template<class Class, class Super> class dash_exportable extends : public Super {
public:
    typedef std::shared_ptr<Class> ptr;
    
    typedef extends<Class, Super> upper;
    typedef Super                 base ;

public:
    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }
};

} //end dash.
