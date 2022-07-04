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
        return std::make_shared<object>();
    }
};

template<class Class, class Super> class d_exportable extends : public Super {
public:
    typedef std::shared_ptr<Class> ptr;
    
    typedef extends<Class, Super> upper;
    typedef Super                 base ;

public:
    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }

public:
    using Super::Super;
};

} //end dash.
