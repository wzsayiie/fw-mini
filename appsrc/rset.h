#pragma once

#include "robject.h"

namespace reflect {

template<> struct type_name_of<class base_set> {
    static constexpr const char *const name = "base_set";
};

class base_set : public extends<base_set, object> {
public:
    virtual void insert(const any &value) = 0;
    virtual void erase (const any &value) = 0;
    virtual bool has   (const any &value) const = 0;
    virtual int  size  () const = 0;
};

template<class Value> class set : public extends<set<Value>, base_set> {
public:
    std::set<Value> set;

    void insert(const any &value) override {
        this->set.insert((Value) value);
    }

    void erase(const any &value) override {
        this->set.erase((Value) value);
    }

    bool has(const any &value) const override {
        return this->set.find((Value) value) != this->set.end();
    }

    int size() const override {
        return (int)this->set.size();
    }
};

} // end reflect.
