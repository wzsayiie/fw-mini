#pragma once

#include "rfunction.h"

namespace reflect {

//function table:

class d_exportable function_table : public dash::extends<function_table, dash::object> {
public:
    void insert(const char *name, const generic_function::ptr &func);
    void erase (const char *name);

    generic_function::ptr find(const char *name);

private:
    std::map<symbol, generic_function::ptr> _functions;
};

d_exportable void inject_function(
    const char *cls_name, const char *fcn_name, const generic_function::ptr &func);

d_exportable void erase_function (const char *cls_name, const char *fcn_name);
d_exportable void erase_functions(const char *cls_name);

//injectable:

template<> struct typeids_of<class injectable> {
    static constexpr const void *value[] = { "injectable", nullptr };
};

class d_exportable injectable : public extends<injectable, object> {
public:
    generic_function::ptr find_injected_function(const char *name);

    void   set_injected_symbol(const symbol &sym);
    symbol injected_symbol    () const;
    
private:
    symbol _injected_sym;
};

} //end reflect.
