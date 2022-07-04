#pragma once

#include "rtypename.h"

namespace reflect {

//symbol:

class d_exportable symbol {
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

template<class Type> struct symbol_of {
    static symbol value() {
        static symbol sym(typename_of<Type>::value());
        return sym;
    }
};

} //end reflect.
