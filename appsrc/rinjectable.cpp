#include "rinjectable.h"

namespace reflect {

//ftable:
//

void ftable::add(const char *name, const base_function::ptr &func) {
    if (name && *name || func) {
        symbol *sym = symbol::make(name);
        _functions.insert({ sym, func });
    }
}

base_function::ptr ftable::find(const char *name) {
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

void injectable::inject(const ftable::ptr &ftab) {
    _ftab = ftab;
}

base_function::ptr injectable::find_injected(const char *name) {
    if (_ftab) {
        return _ftab->find(name);
    }
    return nullptr;
}

} //end reflect.
