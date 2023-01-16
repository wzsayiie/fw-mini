#pragma once

#include <vector>
#include "rany.h"

namespace reflect {

//base vector:

class generic_vector;

template<> struct typeids_of<generic_vector> {
    static constexpr const void *value[] = { "generic_vector", nullptr };
};

class d_exportable generic_vector
    : public extends<generic_vector, object>
    , public std::vector<any>
{
public:
    void _insert(int index, const any &value) {
        if (0 <= index && index <= (int)this->size()) {
            this->insert(this->begin() + index, value);
        }
    }
    
    void _erase(int index) {
        if (0 <= index && index < (int)this->size()) {
            this->erase(this->begin() + index);
        }
    }

    void _push_back(const any &value) {
        this->push_back(value);
    }

    void _pop_back() {
        if (!this->empty()) {
            this->pop_back();
        }
    }

    void _clear() {
        this->clear();
    }
    
    int _size() const {
        return (int)this->size();
    }

    any _at(int index) const {
        if (0 <= index && index < (int)this->size()) {
            return this->at(index);
        }
        return nullptr;
    }

    any _back() const {
        if (!this->empty()) {
            return this->back();
        }
        return nullptr;
    }
};

//vector:

template<class Value, class Actual> class vector_iterator : public Actual {
public:
    vector_iterator() {
    }

    vector_iterator(const Actual &actual): Actual(actual) {
    }

public:
    Value operator*() {
        return take<Value>::from(this->Actual::operator*());
    }

    vector_iterator operator+(ptrdiff_t diff) {
        return this->Actual::operator+(diff);
    }
};

template<class> class vector;

template<class Value> struct typeids_of<vector<Value>> {
    static constexpr const void *value[] = {
        "vector<", typeids_esc, typeids_of<Value>::value, ">", nullptr
    };
};

template<class Value> class vector : public extends<vector<Value>, generic_vector> {
public:
    typedef vector_iterator<Value, typename std::vector<any>::iterator>         iterator;
    typedef vector_iterator<Value, typename std::vector<any>::reverse_iterator> reverse_iterator;

public:
    iterator         begin () { return this->std::vector<any>::begin (); }
    iterator         end   () { return this->std::vector<any>::end   (); }
    reverse_iterator rbegin() { return this->std::vector<any>::rbegin(); }
    reverse_iterator rend  () { return this->std::vector<any>::rend  (); }

    //do not overwrite the functions from std::vector,
    //these functions are commonly used.
    //
    //void insert(index, value)
    //void erase (index)

    void  push_back(const Value &value) { this->_push_back  (value); }
    void  pop_back ()                   { this->_pop_back   ()     ; }
    void  clear    ()                   { this->_clear      ()     ; }
    int   size     ()          const    { return this->_size()     ; }
    Value at       (int index) const    { return this->_at  (index); }
    Value back     ()          const    { return this->_back()     ; }
};

//cast:

template<class Value> struct pointer_cast_as<vector<Value>> {
    //vector<...> has no additional virtual functions, can be converted to each others.

    static typename vector<Value>::ptr from(const object::ptr &obj) {
        auto is_vector = std::dynamic_pointer_cast<generic_vector>(obj);
        if (is_vector) {
            return std::static_pointer_cast<vector<Value>>(obj);
        }
        return nullptr;
    }
};

} //end reflect.
