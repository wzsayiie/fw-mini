#pragma once

#include "dash.h"

namespace reflect {

//symbol:

class dash_exportable symbol {
public:
    static symbol make(const char *str);
    static symbol find(const char *str);

public:
    symbol();
    symbol(const char *str);

public:
    bool operator< (const symbol &that) const;
    bool operator<=(const symbol &that) const;
    bool operator> (const symbol &that) const;
    bool operator>=(const symbol &that) const;
    bool operator==(const symbol &that) const;
    bool operator!=(const symbol &that) const;

    operator const char *() const;
    operator bool        () const;
    
    const char *str() const;
    
private:
    const char *_str;
};

//type symbol:

template<class> struct type_name;

template<class Type> struct type_symbol {
    static symbol value() {
        static symbol sym(type_name<Type>::name);
        return sym;
    }
};

} //end reflect.
