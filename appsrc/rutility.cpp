#include "rutility.h"

namespace reflect {

//symbol:
//

static dash::lazy_var<std::set<symbol>> s_symbols;

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

bool symbol::operator<(const symbol &that) const {
    return strcmp(_str, that._str) < 0;
}

const char *symbol::str() const {
    return _str;
}

symbol::symbol(const char *str) {
    _str = str;
}

//any:
//

any::any() {
}

any::any(nullptr_t) {
}

any::any(bool    value) { _type = data_type::is_bool  ; _word.as_bool   = value; }
any::any(int     value) { _type = data_type::is_int   ; _word.as_int    = value; }
any::any(int64_t value) { _type = data_type::is_int64 ; _word.as_int64  = value; }
any::any(float   value) { _type = data_type::is_float ; _word.as_float  = value; }
any::any(double  value) { _type = data_type::is_double; _word.as_double = value; }

any::any(const std::string &value) {
    _type = data_type::is_string;
    _string = value;
}

any::any(const dash::object::ptr &value) {
    _type = data_type::is_object;
    _object = value;
}

bool any::as_bool() const {
    return !!as_int64() || !_string.empty() || (bool)_object;
}

int any::as_int() const {
    return (int)as_int64();
}

int64_t any::as_int64() const {
    switch (_type) {
        case data_type::is_bool  : return (int64_t)_word.as_bool  ;
        case data_type::is_int   : return (int64_t)_word.as_int   ;
        case data_type::is_int64 : return /* ... */_word.as_int64 ;
        case data_type::is_float : return (int64_t)_word.as_float ;
        case data_type::is_double: return (int64_t)_word.as_double;
            
        default: return 0;
    }
}

float any::as_float() const {
    return (float)as_double();
}

double any::as_double() const {
    switch (_type) {
        case data_type::is_bool  : return (double)_word.as_bool  ;
        case data_type::is_int   : return (double)_word.as_int   ;
        case data_type::is_int64 : return (double)_word.as_int64 ;
        case data_type::is_float : return (double)_word.as_float ;
        case data_type::is_double: return /* .. */_word.as_double;
            
        default: return 0;
    }
}

const std::string &any::as_string() const {
    return _string;
}

const dash::object::ptr &any::as_object() const {
    return _object;
}

any::operator bool   () const { return as_bool  (); }
any::operator int    () const { return as_int   (); }
any::operator int64_t() const { return as_int64 (); }
any::operator float  () const { return as_float (); }
any::operator double () const { return as_double(); }

any::operator const std::string       &() const { return as_string(); }
any::operator const dash::object::ptr &() const { return as_object(); }

} //end reflect.
