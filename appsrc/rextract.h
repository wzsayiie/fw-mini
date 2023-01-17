#pragma once

#include "rfunction.h"
#include "rmap.h"
#include "rmeta.h"
#include "rset.h"
#include "rvector.h"

namespace reflect {

//enum:

template<class Enum> struct extract {
    static_assert(std::is_enum<Enum>::value);
    
    static symbol commit() {
        symbol type = symbol_of<Enum>::value();
        commit_type_meta(type, category::is_enum);
        return type;
    }
};

//basic types:
//basic types are built into the metadata, without calling the "commit" function.

template<> struct extract<any                > { static symbol commit() { return symbol_of<any        >::value(); } };
template<> struct extract<const any         &> { static symbol commit() { return symbol_of<any        >::value(); } };
template<> struct extract<void               > { static symbol commit() { return symbol_of<void       >::value(); } };
template<> struct extract<bool               > { static symbol commit() { return symbol_of<bool       >::value(); } };
template<> struct extract<uint8_t            > { static symbol commit() { return symbol_of<uint8_t    >::value(); } };
template<> struct extract<int                > { static symbol commit() { return symbol_of<int        >::value(); } };
template<> struct extract<int64_t            > { static symbol commit() { return symbol_of<int64_t    >::value(); } };
template<> struct extract<float              > { static symbol commit() { return symbol_of<float      >::value(); } };
template<> struct extract<double             > { static symbol commit() { return symbol_of<double     >::value(); } };
template<> struct extract<char               > { static symbol commit() { return symbol_of<char       >::value(); } };
template<> struct extract<const char        *> { static symbol commit() { return symbol_of<char       >::value(); } };
template<> struct extract<std::string        > { static symbol commit() { return symbol_of<std::string>::value(); } };
template<> struct extract<const std::string &> { static symbol commit() { return symbol_of<std::string>::value(); } };

//function arguments:

template<class> struct arg_appender;

template<class First, class... Args> struct arg_appender<void (First, Args...)> {
    static void append(function_meta *meta) {
        meta->arg_quals.push_back(qualifier_of<First>::value);
        meta->arg_types.push_back(extract<First>::commit());
        
        arg_appender<void (Args...)>::append(meta);
    }
};

template<> struct arg_appender<void ()> {
    static void append(function_meta *) {
    }
};

//function:

template<class Ret, class... Args> struct extract<std::shared_ptr<function<Ret (Args...)>>> {
    static symbol commit() {
        auto type = symbol_of<function<Ret (Args...)>>::value();
        auto meta = (function_meta *)commit_type_meta(type, category::is_function);
        if (!meta) {
            return type;
        }
        
        //arguments.
        arg_appender<void (Args...)>::append(meta);
            
        //returning.
        meta->ret_qual = qualifier_of<Ret>::value;
        meta->ret_type = extract<Ret>::commit();
        
        return type;
    }
};

template<class Ret, class... Args> struct extract<const std::shared_ptr<function<Ret (Args...)>> &> {
    static symbol commit() {
        return extract<typename function<Ret (Args...)>::ptr>::commit();
    }
};

//vector:

template<class Value> struct extract<std::shared_ptr<vector<Value>>> {
    static symbol commit() {
        auto type = symbol_of<vector<Value>>::value();
        auto meta = (vector_meta *)commit_type_meta(type, category::is_vector);
        if (!meta) {
            return type;
        }
        
        meta->value_type = extract<Value>::commit();
        meta->value_qual = qualifier_of<Value>::value;

        return type;
    }
};

template<class Value> struct extract<const std::shared_ptr<vector<Value>> &> {
    static symbol commit() {
        return extract<typename vector<Value>::ptr>::commit();
    }
};

//map:

template<class Key, class Value> struct extract<std::shared_ptr<map<Key, Value>>> {
    static symbol commit() {
        auto type = symbol_of<map<Key, Value>>::value();
        auto meta = (map_meta *)commit_type_meta(type, category::is_map);
        if (!meta) {
            return type;
        }
        
        meta->key_type   = extract<Key  >::commit();
        meta->value_type = extract<Value>::commit();
        meta->key_qual   = qualifier_of<Key  >::value;
        meta->value_qual = qualifier_of<Value>::value;

        return type;
    }
};

template<class Key, class Value> struct extract<const std::shared_ptr<map<Key, Value>> &> {
    static symbol commit() {
        return extract<typename map<Key, Value>::ptr>::commit();
    }
};

//set:

template<class Value> struct extract<std::shared_ptr<set<Value>>> {
    static symbol commit() {
        auto type = symbol_of<set<Value>>::value();
        auto meta = (set_meta *)commit_type_meta(type, category::is_set);
        if (!meta) {
            return type;
        }
        
        meta->value_type = extract<Value>::commit();
        meta->value_qual = qualifier_of<Value>::value;

        return type;
    }
};

template<class Value> struct extract<const std::shared_ptr<set<Value>> &> {
    static symbol commit() {
        return extract<typename set<Value>::ptr>::commit();
    }
};

//class base:

template<class Class> struct base_of {
    typedef std::shared_ptr<typename Class::base> type;
};

template<> struct base_of<reflect::object> {
    typedef void type;
};

//class creator:

template<class Class, bool Abstract> struct creator_of;

template<class Class> struct creator_of<Class, false> {
    static object::ptr value() {
        return function<object::ptr ()>::create([]() {
            return Class::create();
        });
    }
};

template<class Class> struct creator_of<Class, true> {
    static object::ptr value() {
        return nullptr;
    }
};

//class:

template<class Class> struct extract<Class *> {
    static_assert(std::is_class<Class>::value);
    
    static symbol commit() {
        auto type = symbol_of<Class>::value();
        auto meta = (class_meta *)commit_type_meta(type, category::is_class);
        if (!meta) {
            return type;
        }
        
        //base class.
        meta->base_type = extract<typename base_of<Class>::type>::commit();
        
        //if abstract.
        constexpr bool abstract = std::is_abstract<Class>::value;
        meta->abstract = abstract;

        //add static create function.
        if (!abstract) {
            variable constructor;
            constructor.type  = extract<typename function<object::ptr ()>::ptr>::commit();
            constructor.value = creator_of<Class, abstract>::value();
            
            meta->static_functions["create"] = constructor;
        }

        return type;
    }
};

template<class Class> struct extract<const std::shared_ptr<Class> &> {
    static symbol commit() {
        return extract<Class *>::commit();
    }
};

template<class Class> struct extract<std::shared_ptr<Class>> {
    static symbol commit() {
        return extract<Class *>::commit();
    }
};

} //end reflect.
