#pragma once

#include "rfunction.h"

namespace reflect {

//function table:

class dash_exportable function_table : public dash::extends<function_table, dash::object> {
public:
    void insert(const char *name, const base_function::ptr &func);
    void append(const function_table::ptr &table);

    base_function::ptr find(const char *name);

private:
    std::map<symbol, base_function::ptr> _functions;
};

dash_exportable void inject(const char *class_name, const function_table::ptr &table);

//injectable:

template<> struct typeids_of<class injectable> {
    static constexpr const void *value[] = { "injectable", nullptr };
};

class dash_exportable injectable : public extends<injectable, object> {
public:
    base_function::ptr find_injected(const char *name);
    void set_class_symbol(const symbol &sym);
    
private:
    symbol _cls_sym;
};

} //end reflect.
