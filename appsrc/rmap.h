#pragma once

#include "rany.h"
#include "robject.h"

namespace reflect {

template<> struct type_name<class base_map> {
    static constexpr const char *const name = "base_map";
};

class dash_exportable base_map : public extends<base_map, object> {
public:
    virtual void _insert(const any &k, const any &v) = 0;
    virtual void _erase (const any &k)               = 0;
    
    virtual bool _has (const any &k) const = 0;
    virtual int  _size()             const = 0;
    
    virtual void _begin() = 0;
    virtual bool _on   () = 0;
    virtual void _next () = 0;
    virtual any  _key  () = 0;
    virtual any  _value() = 0;
};

template<class Key, class Value> class map
    : public extends<map<Key, Value>, base_map>
    , public std::map<Key, Value>
{
public:
    template<class... Args> map(Args... args): std::map<Key, Value>(args...) {
    }
    
public:
    void _insert(const any &k, const any &v) override { this->insert({ (Key)k, (Value)v }); }
    void _erase (const any &k)               override { this->erase ((Key)k);               }
    
    bool _has (const any &k) const override { return this->find((Key)k) != this->end(); }
    int  _size()             const override { return (int)this->size();                 }
    
    void _begin() override { _it = this->begin();       }
    bool _on   () override { return _it != this->end(); }
    void _next () override { ++_it;                     }
    any  _key  () override { return _it->first;         }
    any  _value() override { return _it->second;        }
    
private:
    typename std::map<Key, Value>::iterator _it;
};

} //end reflect.
