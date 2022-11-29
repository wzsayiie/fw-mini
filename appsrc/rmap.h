#pragma once

#include "rany.h"

namespace reflect {

//base map:

class base_map;

template<> struct typeids_of<base_map> {
    static constexpr const void *value[] = { "base_map", nullptr };
};

class d_exportable base_map
    : public extends<base_map, object>
    , public std::map<any, any>
{
public:
    void _insert(const any &key, const any &value) {
        this->insert(std::pair<any, any>(key, value));
    }

    void _erase(const any &key) {
        this->erase(key);
    }

    void _clear() {
        this->clear();
    }
    
    bool _has(const any &key) const {
        return this->find(key) != this->end();
    }

    any _at(const any &key) const {
        auto it = this->find(key);
        if (it != this->end()) {
            return it->second;
        }
        return nullptr;
    }

    int _size() const {
        return (int)this->size();
    }
    
    void _begin() { _iterator = this->begin();       }
    bool _on   () { return _iterator != this->end(); }
    void _next () { ++_iterator;                     }
    any  _key  () { return _iterator->first;         }
    any  _value() { return _iterator->second;        }
    
private:
    typename std::map<any, any>::iterator _iterator;
};

//map:

template<class Key, class Value, class Actual> class pair : public Actual {
public:
    pair() {
    }

    pair(const Actual &actual): Actual(actual) {
        this->first  = take<Key  >::from(this->Actual::first );
        this->second = take<Value>::from(this->Actual::second);
    }

public:
    Key   first ;
    Value second;
};

template<class Key, class Value, class Actual> class map_iterator : public Actual {
public:
    map_iterator() {
    }

    map_iterator(const Actual &actual): Actual(actual) {
    }

public:
    pair<Key, Value, std::pair<const any, any>> operator*() {
        return this->Actual::operator*();
    }
};

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

template<class Key, class Value> class map : public extends<map<Key, Value>, base_map> {
public:
    typedef map_iterator<Key, Value, typename std::map<any, any>::iterator> iterator;

public:
    iterator begin() { return this->std::map<any, any>::begin(); }
    iterator end  () { return this->std::map<any, any>::end  (); }

    void  insert(const Key &key, const Value &value) { this->_insert(key, value); }
    void  erase (const Key &key)                     { this->_erase (key)       ; }
    void  clear ()                                   { this->_clear ()          ; }
    bool  has   (const Key &key) const               { return this->_has (key)  ; }
    Value at    (const Key &key) const               { return this->_at  (key)  ; }
    int   size  ()               const               { return this->_size()     ; }
};

} //end reflect.
