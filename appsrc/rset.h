#pragma once

#include "robject.h"

namespace reflect {

reflect_reflectable_class(base_set);
class base_set : extends<base_set, object> {
public:
    virtual void insert(const any &value) = 0;
    virtual void erase (const any &value) = 0;
    virtual bool has   (const any &value) const = 0;
    virtual int  size  () const = 0;
};

template<class Value> struct set : extends<set<Value>, base_set> {
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

//builtin set types.
reflect_reflectable_class(set<bool>       );
reflect_reflectable_class(set<int>        );
reflect_reflectable_class(set<int64_t>    );
reflect_reflectable_class(set<float>      );
reflect_reflectable_class(set<double>     );
reflect_reflectable_class(set<std::string>);

} // end reflect.
