#pragma once

#include "rany.h"

namespace reflect {

//base vector:

class base_vector;

template<> struct typeids_of<base_vector> {
    static constexpr const void *value[] = { "base_vector", nullptr };
};

class d_exportable base_vector : public extends<base_vector, object> {
public:
    virtual void _insert   (int i, const any &v) = 0;
    virtual void _erase    (int i)               = 0;
    virtual void _push_back(const any &v)        = 0;
    virtual void _pop_back ()                    = 0;
    virtual void _clear    ()                    = 0;
    
    virtual int _size ()      const = 0;
    virtual any _at   (int i) const = 0;
    virtual any _back ()      const = 0;
};

//vector:

template<class> class vector;

template<class Value> struct typeids_of<vector<Value>> {
    static constexpr const void *value[] = {
        "vector<", typeids_esc, typeids_of<Value>::value, ">", nullptr
    };
};

template<class Value> class vector
    : public extends<vector<Value>, base_vector>
    , public std::vector<Value>
{
public:
    template<class... Args> vector(Args...args): std::vector<Value>(args...) {
    }

public:
    void _insert(int i, const any &v) override {
        if (0 <= i && i <= (int)this->size()) {
            this->insert(this->begin() + i, take<Value>::from(v));
        }
    }
    
    void _erase(int i) override {
        if (0 <= i && i < (int)this->size()) {
            this->erase(this->begin() + i);
        }
    }

    void _push_back(const any &v) override {
        this->push_back(take<Value>::from(v));
    }

    void _pop_back() override {
        if (!this->empty()) {
            this->pop_back();
        }
    }

    void _clear() override {
        this->clear();
    }
    
    int _size() const override {
        return (int)this->size();
    }

    any _at(int i) const override {
        if (0 <= i && i < (int)this->size()) {
            return this->at(i);
        }
        return nullptr;
    }

    any _back() const override {
        if (!this->empty()) {
            return this->back();
        }
        return nullptr;
    }
};

} //end reflect.
