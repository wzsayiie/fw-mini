#include "rsymbol.h"
#include "dash.h"

namespace reflect {

static dash::lazy<std::set<symbol>> s_symbols;

symbol *symbol::make(const char *str) {
    if (!str || !*str) {
        return nullptr;
    }
    
    symbol sym(str);
    
    auto the = s_symbols->find(sym);
    if (the == s_symbols->end()) {
        return (symbol *)&*s_symbols->insert(sym).first;
    } else {
        return (symbol *)&*the;
    }
}

symbol *symbol::find(const char *str) {
    if (!str || !*str) {
        return nullptr;
    }
    
    auto the = s_symbols->find(symbol(str));
    if (the != s_symbols->end()) {
        return (symbol *)&*the;
    } else {
        return nullptr;
    }
}

bool symbol::operator<(const symbol &that) const {
    return strcmp(_str, that._str) < 0;
}

const char *symbol::str() const {
    return _str;
}

symbol::symbol(const char *str) {
    _str = str;
}

} //end reflect.
