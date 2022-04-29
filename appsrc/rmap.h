#pragma once

#include "rany.h"
#include "robject.h"

namespace reflect {

template<> struct type_name_of<class base_map> {
    static constexpr const char *const name = "base_map";
};

class base_map : public extends<base_map, object> {
public:
    virtual void insert(const any &key, const any &value) = 0;
    virtual void erase (const any &key) = 0;
    
    virtual bool has(const any &key) const = 0;
    virtual int size() const = 0;
    
    virtual void reset    () = 0;
    virtual bool move_next() = 0;
    virtual any  cur_key  () = 0;
    virtual any  cur_value() = 0;
};

template<class Key, class Value> class map : public extends<map<Key, Value>, base_map> {
public:
    std::map<Key, Value> map;
    
    void insert(const any &key, const any &value) override {
        this->map.insert({ (Key)key, (Value)value });
    }
    
    void erase(const any &key) override {
        this->map.erase((Key)key);
    }
    
    bool has(const any &key) const override {
        return this->map.find((Key)key) != this->map.end();
    }
    
    int size() const override {
        return (int)this->map.size();
    }
    
    void reset() override {
        _it = this->map.begin();
    }
    
    bool move_next() override {
        return ++_it != this->map.end();
    }
    
    any cur_key  () override { return _it->first ; }
    any cur_value() override { return _it->second; }
    
private:
    typename std::map<Key, Value>::iterator _it;
};

} //end reflect.
