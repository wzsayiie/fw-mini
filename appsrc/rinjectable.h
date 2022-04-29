#pragma once

#include "rfunction.h"

namespace reflect {

//function table:

class function_table : public dash::extends<function_table, dash::object> {
public:
    void insert(const char *name, const base_function::ptr &func);
    void append(const function_table::ptr &that);

    base_function::ptr find(const char *name);

private:
    std::map<symbol, base_function::ptr> _functions;
};

void inject(const char *class_name, const function_table::ptr &table);

//injectable:

template<> struct type_name<class injectable> {
    static constexpr const char *const name = "injectable";
};

class injectable : public extends<injectable, object> {
public:
    base_function::ptr find_injected(const char *name);
};

} //end reflect.
