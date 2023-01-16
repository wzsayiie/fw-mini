#include "rsymbol.h"
#include <set>
#include "dlazy.h"

namespace reflect {

static dash::lazy<std::set<std::string>> s_strings;

symbol symbol::make(const char *str) {
    return str;
}

symbol symbol::find(const char *str) {
    if (str && *str) {
        auto it = s_strings->find(str);
        if (it != s_strings->end()) {
            return it->c_str();
        }
    }
    return nullptr;
}

symbol::symbol() {
    _str = nullptr;
}

symbol::symbol(const char *str) {
    if (str && *str) {
        auto pair = s_strings->insert(str);
        _str = pair.first->c_str();
    } else {
        _str = nullptr;
    }
}

bool symbol::operator< (const symbol &that) const { return _str <  that._str; }
bool symbol::operator<=(const symbol &that) const { return _str <= that._str; }
bool symbol::operator> (const symbol &that) const { return _str >  that._str; }
bool symbol::operator>=(const symbol &that) const { return _str >= that._str; }
bool symbol::operator==(const symbol &that) const { return _str == that._str; }
bool symbol::operator!=(const symbol &that) const { return _str != that._str; }

symbol::operator const char *() const { return _str; }
symbol::operator bool        () const { return _str; }

const char *symbol::str() const {
    return _str;
}

} //end reflect.
