#include "rinjectable.h"

namespace reflect {

//function_table:
//

void function_table::add(const char *name, const base_function::ptr &func) {
    if (name && *name && func) {
        symbol *sym = symbol::make(name);
        _functions.insert({ sym, func });
    }
}

base_function::ptr function_table::find(const char *name) {
    symbol *sym = symbol::find(name);

    auto target = _functions.find(sym);
    if (target != _functions.end()) {
        return target->second;
    } else {
        return nullptr;
    }
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
