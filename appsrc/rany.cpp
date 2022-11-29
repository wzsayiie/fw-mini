#include "rany.h"

namespace reflect {

any::any() {
}

any::any(nullptr_t) {
}

any::any(bool    value) { _type = data_type::is_bool  ; _word.as_bool   = value; }
any::any(uint8_t value) { _type = data_type::is_byte  ; _word.as_byte   = value; }
any::any(int     value) { _type = data_type::is_int   ; _word.as_int    = value; }
any::any(int64_t value) { _type = data_type::is_int64 ; _word.as_int64  = value; }
any::any(float   value) { _type = data_type::is_float ; _word.as_float  = value; }
any::any(double  value) { _type = data_type::is_double; _word.as_double = value; }

any::any(const char *value) {
    _type   = data_type::is_string;
    _string = value ? value : "";
}

any::any(const std::string &value) {
    _type   = data_type::is_string;
    _string = value;
}

any::any(object *value) {
    _type = data_type::is_object;
    if (value) {
        _object = value->me();
    }
}

any::any(const object::ptr &value) {
    _type   = data_type::is_object;
    _object = value;
}

data_type any::preferred_type() const {
    return _type;
}

bool any::is_numeric_type() const {
    switch (_type) {
        case data_type::is_bool  :
        case data_type::is_byte  :
        case data_type::is_int   :
        case data_type::is_int64 :
        case data_type::is_float :
        case data_type::is_double: return true;

        default: return false;
    }
}

bool any::as_bool() const {
    return !!as_int64() || !_string.empty() || (bool)_object;
}

uint8_t any::as_byte() const { return (uint8_t)as_int64(); }
int     any::as_int () const { return (int    )as_int64(); }

int64_t any::as_int64() const {
    switch (_type) {
        case data_type::is_bool  : return (int64_t)_word.as_bool  ;
        case data_type::is_byte  : return (int64_t)_word.as_byte  ;
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
        case data_type::is_byte  : return (double)_word.as_byte  ;
        case data_type::is_int   : return (double)_word.as_int   ;
        case data_type::is_int64 : return (double)_word.as_int64 ;
        case data_type::is_float : return (double)_word.as_float ;
        case data_type::is_double: return /* .. */_word.as_double;
            
        default: return 0;
    }
}

const char *any::as_chars        () const { return _string.c_str(); }
std::string any::as_string       () const { return _string        ; }
object *    any::as_object_ptr   () const { return _object.get()  ; }
object::ptr any::as_object_shared() const { return _object        ; }

any::operator bool        () const { return as_bool         (); }
any::operator uint8_t     () const { return as_byte         (); }
any::operator int         () const { return as_int          (); }
any::operator int64_t     () const { return as_int64        (); }
any::operator float       () const { return as_float        (); }
any::operator double      () const { return as_double       (); }
any::operator const char *() const { return as_chars        (); }
any::operator std::string () const { return as_string       (); }
any::operator object *    () const { return as_object_ptr   (); }
any::operator object::ptr () const { return as_object_shared(); }

int any::compare(const any &that) const {
    if (_type == data_type::is_void) {
        return that._type != data_type::is_void ? -1 : 0;
    }

    if (is_numeric_type()) {
        if (that.is_numeric_type()) {
            double a = this->as_double();
            double b = that. as_double();
            
            if (a != b) {
                return a < b ? -1 : 1;
            }
            return 0;

        } else {
            return -1;
        }
    }

    if (_type == data_type::is_string) {
        if (that._type == data_type::is_string) {
            return _string.compare(that._string);
        } else {
            return -1;
        }
    }

    //"this" and "that" both are objects.
    if (_object != that._object) {
        return _object < that._object ? -1 : 1;
    }
    return 0;
}

bool any::operator< (const any &that) const { return compare(that) <  0; }
bool any::operator<=(const any &that) const { return compare(that) <= 0; }
bool any::operator> (const any &that) const { return compare(that) >  0; }
bool any::operator>=(const any &that) const { return compare(that) >= 0; }
bool any::operator==(const any &that) const { return compare(that) == 0; }
bool any::operator!=(const any &that) const { return compare(that) != 0; }

} //end reflect.
