#include "rinjectable.h"

namespace reflect {

//function table:

static dash::lazy<std::map<symbol, function_table::ptr>> s_tables;

void function_table::insert(const char *name, const generic_function::ptr &func) {
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

generic_function::ptr function_table::find(const char *name) {
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

void inject(const char *cls_name, const char *fcn_name, const generic_function::ptr &func) {
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

generic_function::ptr injectable::find_injected(const char *name) {
    if (!_injected_sym) {
        return nullptr;
    }

    auto it = s_tables->find(_injected_sym);
    if (it == s_tables->end()) {
        return nullptr;
    }

    return it->second->find(name);
}

void injectable::set_injected_symbol(const symbol &sym) {
    _injected_sym = sym;
}

symbol injectable::injected_symbol() const {
    return _injected_sym;
}

} //end reflect.
