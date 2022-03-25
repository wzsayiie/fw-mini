#pragma once

#include "robject.h"

namespace reflect {

template<> struct type_name_of<class base_vector> {
    static constexpr const char *const name = "base_vector";
};

class base_vector : public extends<base_vector, object> {
public:
    virtual void insert(int index, const any &value) = 0;
    virtual void erase (int index) = 0;
    
    virtual void push_back(const any &value) = 0;
    virtual void pop_back () = 0;
    
    virtual any at(int index) const = 0;
    virtual any front() const = 0;
    virtual any back () const = 0;
    
    virtual int size() const = 0;
};

template<class Value> class vector : public extends<vector<Value>, base_vector> {
public:
    std::vector<Value> vector;
    
    void insert(int index, const any &value) override {
        this->vector.insert(this->vector.begin() + index, (Value)value);
    }
    
    void erase(int index) override {
        this->vector.erase(this->vector.begin() + index);
    }
    
    void push_back(const any &value) override {
        this->vector.push_back(value);
    }
    
    void pop_back() override {
        this->vector.pop_back();
    }
    
    any at(int index) const override {
        return this->vector.at(index);
    }
    
    any front() const override { return this->vector.front(); }
    any back () const override { return this->vector.back (); }
    
    int size() const override {
        return (int)this->vector.size();
    }
};

} // end reflect.
