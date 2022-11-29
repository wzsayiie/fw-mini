#pragma once

#include "rany.h"

namespace reflect {
    
//base set:

class base_set;

template<> struct typeids_of<base_set> {
    static constexpr const void *value[] = { "base_set", nullptr };
};

class d_exportable base_set
    : public extends<base_set, object>
    , public std::set<any>
{
public:
    void _insert(const any &value) {
        this->insert(value);
    }

    void _erase(const any &value) {
        this->erase(value);
    }

    void _clear() {
        this->clear();
    }

    bool _has(const any &value) const {
        return this->find(value) != this->end();
    }
    
    int _size() const {
        return (int)this->size();
    }
    
    void _begin() { _iterator = this->begin();       }
    bool _on   () { return _iterator != this->end(); }
    void _next () { ++_iterator;                     }
    any  _value() { return *_iterator;               }
    
private:
    typename std::set<any>::iterator _iterator;
};

//set:

template<class Value, class Actual> class set_iterator : public Actual {
public:
    set_iterator() {
    }

    set_iterator(const Actual &actual): Actual(actual) {
    }

public:
    Value operator*() {
        return take<Value>::from(this->Actual::operator*());
    }
};

template<class> class set;

template<class Value> struct typeids_of<set<Value>> {
    static constexpr const void *value[] = {
        "set<", typeids_esc, typeids_of<Value>::value, ">", nullptr
    };
};

template<class Value> class set : public extends<set<Value>, base_set> {
public:
    typedef set_iterator<Value, typename std::set<any>::iterator> iterator;

public:
    iterator begin() { return this->std::set<any>::begin(); }
    iterator end  () { return this->std::set<any>::end  (); }

    void insert(const Value &value)       { this->_insert(value)     ; }
    void erase (const Value &value)       { this->_erase (value)     ; }
    void clear ()                         { this->_clear ()          ; }
    bool has   (const Value &value) const { return this->_has (value); }
    int  size  ()                   const { return this->_size()     ; }
};

} //end reflect.
