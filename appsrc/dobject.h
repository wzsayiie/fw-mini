#pragma once

#include <memory>

namespace dash {

//pod:

struct pod_object {
};

//virtual:

class virtual_object {
public:
    virtual ~virtual_object() = default;
};

class object : public virtual_object {
public:
    typedef std::shared_ptr<object> ptr;

    static ptr create() {
        return std::make_shared<object>();
    }
};

template<class Class, class Super> class extends : public Super {
public:
    typedef std::shared_ptr<Class> ptr;

    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }

public:
    using   Super::Super;
    typedef extends<Class, Super> base;
};

} //end dash.