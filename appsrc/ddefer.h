#pragma once

#include <functional>
#include "dname.h"

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

#define d_defer dash::defer d_name = [&]()
