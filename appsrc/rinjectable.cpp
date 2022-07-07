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

static function_table::ptr injected_table_of(const char *cls_name) {
    symbol sym = symbol::make(cls_name);
    if (!sym) {
        return nullptr;
    }

    auto found = s_tables->find(sym);
    if (found != s_tables->end()) {
        return found->second;
    }

    auto created  = function_table::create();
    auto inserted = s_tables->insert({ sym, created });
    return inserted.first->second;
}

void inject(const char *cls_name, const char *fcn_name, const base_function::ptr &func) {
    function_table::ptr table = injected_table_of(cls_name);
    if (table) {
        table->insert(fcn_name, func);
    }
}

void inject(const char *cls_name, const function_table::ptr &table) {
    function_table::ptr present = injected_table_of(cls_name);
    if (present) {
        present->append(table);
    }
}

//injectable:

base_function::ptr injectable::find_injected(const char *name) {
    auto it = s_tables->find(_cls_sym ? _cls_sym : class_symbol());
    if (it != s_tables->end()) {
        return it->second->find(name);
    }

    return nullptr;
}

void injectable::set_class_symbol(const symbol &sym) {
    _cls_sym = sym;
}

} //end reflect.
