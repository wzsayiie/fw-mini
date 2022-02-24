#include "rmeta.h"

namespace reflect {

static dash::lazy_var<std::map<symbol *, function_meta *>> s_function_map;
static dash::lazy_var<std::map<symbol *, class_meta    *>> s_class_map   ;
static dash::lazy_var<std::map<symbol *, basic_meta    *>> s_basic_map   ;

static dash::lazy_var<std::vector<function_meta *>> s_function_seq;
static dash::lazy_var<std::vector<class_meta    *>> s_class_seq   ;
static dash::lazy_var<std::vector<basic_meta    *>> s_basic_seq   ;

static usrclass_meta *get_usrclass(symbol *name) {
    auto it = s_class_map->find(name);
    if (it != s_class_map->end()) {
        return (usrclass_meta *) it->second;
    }

    auto fresh = new usrclass_meta();
    fresh->type = type_category::is_usrclass;
    fresh->name = name;

    s_class_map->insert({name, fresh});
    s_class_seq->push_back(fresh);

    return fresh;
}

void commit_meta(basic_meta *meta) {
    switch (meta->type) {
    case type_category::is_function:
        {
            auto real = (function_meta *)meta;
            if (real->belong_class) {
                //is class function.
                usrclass_meta *belong = get_usrclass(real->belong_class);
                belong->function_map.insert({real->name, real});
                belong->function_seq.push_back(real);

            } else {
                //is global function.
                s_function_map->insert({real->name, real});
                s_function_seq->push_back(real);
            }
        }
        break;

    case type_category::is_map:
    case type_category::is_set:
    case type_category::is_vector:
        {
            auto real = (class_meta *)meta;
            s_class_map->insert({real->name, real});
            s_class_seq->push_back(real);
        }
        break;

    case type_category::is_usrclass:
        {
            get_usrclass(meta->name);
        }
        break;

    default:
        {
            s_basic_map->insert({meta->name, meta});
            s_basic_seq->push_back(meta);
        }
    }
}

template<class Meta> const Meta *find(
    const dash::lazy_var<std::map<symbol *, Meta *>> &map, const char *name)
{
    auto it = map->find(symbol::find(name));
    if (it != map->end()) {
        return it->second;
    }
    return nullptr;
}

const function_meta *find_function(const char *name) { return find(s_function_map, name); }
const class_meta    *find_class   (const char *name) { return find(s_class_map   , name); }
const basic_meta    *find_basic   (const char *name) { return find(s_basic_map   , name); }

const std::vector<function_meta *> &functions() { return *s_function_seq; }
const std::vector<class_meta    *> &classes  () { return *s_class_seq   ; }
const std::vector<basic_meta    *> &basics   () { return *s_basic_seq   ; }

} // end reflect.
