#pragma once

#include "rfunction.h"

namespace reflect {

//function table:

class d_exportable function_table : public dash::extends<function_table, dash::object> {
public:
    void insert(const char *name, const generic_function::ptr &func);
    void append(const function_table::ptr &table);

    generic_function::ptr find(const char *name);

private:
    std::map<symbol, generic_function::ptr> _functions;
};

d_exportable void inject(const char *cls_name, const char *fcn_name, const generic_function::ptr &func);
d_exportable void inject(const char *cls_name, const function_table::ptr &table);

//injectable:

template<> struct typeids_of<class injectable> {
    static constexpr const void *value[] = { "injectable", nullptr };
};

class d_exportable injectable : public extends<injectable, object> {
public:
    generic_function::ptr find_injected(const char *name);
    void set_class_symbol(const symbol &sym);
    
private:
    symbol _cls_sym;
};

} //end reflect.
