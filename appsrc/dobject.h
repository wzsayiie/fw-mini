#pragma once

#include "dexport.h"
#include "dstd.h"

namespace dash {

//virtual:

class d_exportable virtual_object {
public:
    virtual ~virtual_object() = default;
};

//object:

class d_exportable object : public virtual_object {
public:
    typedef std::shared_ptr<object> ptr;

public:
    static ptr create() {
        ptr obj = std::make_shared<object>();
        obj->_weak = obj;
        return obj;
    }

public:
    ptr shared() const {
        return _weak.lock();
    }

protected:
    std::weak_ptr<object> _weak;
};

template<class Class, class Super> class d_exportable extends : public Super {
public:
    typedef std::shared_ptr<Class> ptr;
    
    typedef extends<Class, Super> upper;
    typedef Super                 base ;

public:
    template<class... Args> static ptr create(Args... args) {
        ptr obj = std::make_shared<Class>(args...);
        obj->_weak = obj;
        return obj;
    }

public:
    using Super::Super;

public:
    ptr shared() const {
        return std::static_pointer_cast<Class>(this->_weak.lock());
    }
};

} //end dash.
