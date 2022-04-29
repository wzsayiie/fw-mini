#include "rinjectable.h"

namespace reflect {

//function_table:
//

void function_table::add(const char *name, const base_function::ptr &func) {
}

base_function::ptr function_table::find(const char *name) {
    return nullptr;
}

//injectable:
//

void injectable::inject(const function_table::ptr &table) {
    _table = table;
}

base_function::ptr injectable::find_injected(const char *name) {
    if (_table) {
        return _table->find(name);
    }
    return nullptr;
}

} //end reflect.
