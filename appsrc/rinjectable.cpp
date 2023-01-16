#include "rinjectable.h"
#include "dlazy.h"

namespace reflect {

//function table:

static dash::lazy<std::map<symbol, function_table::ptr>> s_tables;

void function_table::insert(const char *name, const generic_function::ptr &func) {
    if (!func) {
        return;
    }

    symbol sym = symbol::make(name);
    if (!sym) {
        return;
    }

    _functions.insert({ sym, func });
}

void function_table::erase(const char *name) {
    symbol sym = symbol::find(name);
    if (sym) {
        _functions.erase(sym);
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

void inject_class_function(
    const char *cls_name, const char *fcn_name, const generic_function::ptr &func)
{
    if (!func) {
        return;
    }

    symbol cls_sym = symbol::make(cls_name);
    if (!cls_sym) {
        return;
    }

    function_table::ptr table;

    auto found = s_tables->find(cls_sym);
    if (found == s_tables->end()) {
        table = function_table::create();
        s_tables->insert({ cls_sym, table });
    } else {
        table = found->second;
    }

    table->insert(fcn_name, func);
}

void erase_class_function(const char *cls_name, const char *fcn_name) {
    symbol cls_sym = symbol::find(cls_name);
    if (!cls_sym) {
        return;
    }

    auto table = s_tables->find(cls_sym);
    if (table == s_tables->end()) {
        return;
    }

    table->second->erase(fcn_name);
}

void erase_class_functions(const char *cls_name) {
    symbol sym = symbol::find(cls_name);
    if (sym) {
        s_tables->erase(sym);
    }
}

//injectable:

generic_function::ptr injectable::find_injected_function(const char *name) {
    //find own injected object functions firstly.
    if (_injected_fcns) {
        generic_function::ptr func = _injected_fcns->find(name);
        if (func) {
            return func;
        }
    }
    
    //find injected class functions.
    //NOTE: it is also necessary to consider the class symbol.
    symbol cls_sym = _injected_sym ? _injected_sym : class_symbol();

    auto pair = s_tables->find(cls_sym);
    if (pair != s_tables->end()) {
        return pair->second->find(name);
    }

    return nullptr;
}

void injectable::inject_function(const char *name, const generic_function::ptr &func) {
    if (!_injected_fcns) {
        _injected_fcns = function_table::create();
    }
    _injected_fcns->insert(name, func);
}

void injectable::erase_function(const char *name) {
    if (_injected_fcns) {
        _injected_fcns->erase(name);
    }
}

void injectable::set_injected_symbol(const symbol &sym) {
    _injected_sym = sym;
}

symbol injectable::injected_symbol() const {
    return _injected_sym;
}

void injectable::dispose() {
    if (!_disposed) {
        _disposed = true;
        on_dispose();
    }
}

void injectable::on_dispose() {
    _injected_fcns = nullptr;
}

} //end reflect.
