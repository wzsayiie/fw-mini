#pragma once

#include "rany.h"

namespace reflect {

//base vector:

class base_vector;

template<> struct typeids_of<base_vector> {
    static constexpr const void *value[] = { "base_vector", nullptr };
};

class dash_exportable base_vector : public extends<base_vector, object> {
public:
    virtual void _insert(int i, const any &v) = 0;
    virtual void _erase (int i)               = 0;

    virtual void _push_back(const any &v) = 0;
    virtual void _pop_back ()             = 0;
    
    virtual int _size ()      const = 0;
    virtual any _at   (int i) const = 0;
    virtual any _front()      const = 0;
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
        this->insert(this->begin() + i, take<Value>::from(v));
    }
    void _erase(int i) override {
        this->erase(this->begin() + i);
    }

    void _push_back(const any &v) override {
        this->push_back(take<Value>::from(v));
    }
    void _pop_back() override {
        this->pop_back();
    }
    
    int _size ()      const override { return (int)this->size(); }
    any _at   (int i) const override { return this->at(i);       }
    any _front()      const override { return this->front();     }
    any _back ()      const override { return this->back();      }
};

} //end reflect.
