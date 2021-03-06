#pragma once

#include "rany.h"

namespace reflect {
    
//base set:

class base_set;

template<> struct typeids_of<base_set> {
    static constexpr const void *value[] = { "base_set", nullptr };
};

class d_exportable base_set : public extends<base_set, object> {
public:
    virtual void _insert(const any &v) = 0;
    virtual void _erase (const any &v) = 0;
    
    virtual bool _has(const any &v) const = 0;

    virtual int _size() const = 0;
    
    virtual void _begin() = 0;
    virtual bool _on   () = 0;
    virtual void _next () = 0;
    virtual any  _value() = 0;
};

//set:

template<class> class set;

template<class Value> struct typeids_of<set<Value>> {
    static constexpr const void *value[] = {
        "set<", typeids_esc, typeids_of<Value>::value, ">", nullptr
    };
};

template<class Value> class set
    : public extends<set<Value>, base_set>
    , public std::set<Value>
{
public:
    template<class... Args> set(Args... args): std::set<Value>(args...) {
    }
    
public:
    void _insert(const any &v) override {
        this->insert(take<Value>::from(v));
    }
    void _erase(const any &v) override {
        this->erase(take<Value>::from(v));
    }

    bool _has(const any &v) const override {
        return this->find(take<Value>::from(v)) != this->end();
    }
    
    int _size() const override {
        return (int)this->size();
    }
    
    void _begin() override { _it = this->begin();       }
    bool _on   () override { return _it != this->end(); }
    void _next () override { ++_it;                     }
    any  _value() override { return *_it;               }
    
private:
    typename std::set<Value>::iterator _it;
};

} //end reflect.
