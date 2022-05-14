#include "rinjectable.h"

namespace reflect {

//function table:

static dash::lazy<std::map<symbol, function_table::ptr>> s_tables;

void function_table::insert(const char *name, const base_function::ptr &func) {
    symbol sym = symbol::make(name);
    if (sym && func) {
        _functions.insert({ sym, func });
    }
}

void function_table::append(const function_table::ptr &table) {
    if (table) {
        _functions.insert(table->_functions.begin(), table->_functions.end());
    }
}

base_function::ptr function_table::find(const char *name) {
    symbol sym = symbol::find(name);
    if (!sym) {
        return nullptr;
    }

    auto it = _functions.find(sym);
    if (it == _functions.end()) {
        return nullptr;
    }

    return it->second;
}

void inject(const char *class_name, const function_table::ptr &table) {
    symbol sym = symbol::make(class_name);
    if (!sym) {
        return;
    }

    auto it = s_tables->find(sym);
    if (it == s_tables->end()) {
        auto stored = function_table::create();
        stored->append(table);
        s_tables->insert({ sym, stored });

    } else {
        it->second->append(table);
    }
}

//injectable:

base_function::ptr injectable::find_injected(const char *name) {
    auto it = s_tables->find(_cls_sym ? _cls_sym : class_symbol());
    if (it == s_tables->end()) {
        return nullptr;
    }

    return it->second->find(name);
}

void injectable::set_class_symbol(const symbol &sym) {
    _cls_sym = sym;
}

} //end reflect.
