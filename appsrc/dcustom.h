#pragma once

#include "dstd.h"

namespace dash {

//lazy var:
//

template<class Type> class lazy_var {
public:
    Type *operator->() { return  ptr(); }
    Type &operator* () { return *ptr(); }
    
    Type *ptr() {
        if (!_obj) {
            _obj = new Type();
        }
        return _obj;
    }

private:
    Type *_obj = nullptr;
};

//object:
//

struct object {
    typedef std::shared_ptr<object> ptr;
    virtual ~object() = default;
};

template<class Class, class Super> struct extends : Super {
    typedef std::shared_ptr<Class> ptr;
    
    template<class... Args> static ptr create(Args... args) {
        return std::make_shared<Class>(args...);
    }
};

} //end dash.
