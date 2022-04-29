#pragma once

namespace reflect {

class symbol {
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

template<class> struct type_name_of;

template<class Type> struct type_name {
    static symbol value() {
        static symbol sym(type_name_of<Type>::name);
        return sym;
    }
};

} //end reflect.
