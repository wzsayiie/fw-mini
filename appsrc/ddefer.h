#pragma once

#include "dname.h"
#include "dstd.h"

namespace dash {

class defer {
public:
    template<class Func> defer(const Func &func) {
        _func = func;
    }

    ~defer() {
        _func();
    }

private:
    std::function<void ()> _func;
};

} //end dash.

#define dash_defer dash::defer dash_name = [&]()
