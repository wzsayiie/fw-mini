#pragma once

namespace reflect {

class symbol {
public:
    static symbol *make(const char *str);
    static symbol *find(const char *str);
    
    bool operator<(const symbol &that) const;
    const char *str() const;
    
private:
    symbol(const char *str);
    
    const char *_str;
};

template<class> struct type_name_of;

template<class Type> struct type_name {
    static symbol *value() {
        static symbol *sym = symbol::make(type_name_of<Type>::name);
        return sym;
    }
};

} //end reflect.
