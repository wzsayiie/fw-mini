#pragma once

#include "rany.h"

namespace reflect {

//base map:

class base_map;

template<> struct typeids_of<base_map> {
    static constexpr const void *value[] = { "base_map", nullptr };
};

class d_exportable base_map : public extends<base_map, object> {
public:
    virtual void _insert(const any &k, const any &v) = 0;
    virtual void _erase (const any &k)               = 0;
    virtual void _clear ()                           = 0;
    
    virtual bool _has (const any &k) const = 0;
    virtual any  _get (const any &k) const = 0;
    virtual int  _size()             const = 0;
    
    virtual void _begin() = 0;
    virtual bool _on   () = 0;
    virtual void _next () = 0;
    virtual any  _key  () = 0;
    virtual any  _value() = 0;
};

//map:

template<class, class> class map;

template<class Key, class Value> struct typeids_of<map<Key, Value>> {
    static constexpr const void *value[] = {
        "map<",
        typeids_esc, typeids_of<Key>::value,
        ",",
        typeids_esc, typeids_of<Value>::value,
        ">",
        nullptr
    };
};

template<class Key, class Value> class map
    : public extends<map<Key, Value>, base_map>
    , public std::map<Key, Value>
{
public:
    template<class... Args> map(Args... args): std::map<Key, Value>(args...) {
    }
    
public:
    void _insert(const any &k, const any &v) override {
        this->insert({ take<Key>::from(k), take<Value>::from(v) });
    }

    void _erase(const any &k) override {
        this->erase(take<Key>::from(k));
    }

    void _clear() override {
        this->clear();
    }
    
    bool _has(const any &k) const override {
        return this->find(take<Key>::from(k)) != this->end();
    }

    any _get(const any &k) const override {
        auto it = this->find(take<Key>::from(k));
        if (it != this->end()) {
            return it->second;
        }
        return nullptr;
    }

    int _size() const override {
        return (int)this->size();
    }
    
    void _begin() override { _it = this->begin();       }
    bool _on   () override { return _it != this->end(); }
    void _next () override { ++_it;                     }
    any  _key  () override { return _it->first;         }
    any  _value() override { return _it->second;        }
    
private:
    typename std::map<Key, Value>::iterator _it;
};

} //end reflect.
