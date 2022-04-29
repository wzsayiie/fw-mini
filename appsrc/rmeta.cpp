#include "rmeta.h"

namespace reflect {

static dash::lazy<std::map<symbol, type_meta *>> s_map;
static dash::lazy<std::vector<type_meta *>>      s_seq;

template<class Meta> Meta *get(symbol sym, type_category type) {
    auto it = s_map->find(sym);
    if (it != s_map->end()) {
        return (Meta *)it->second;
    }

    auto fresh = new Meta();
    fresh->type = type;
    fresh->sym  = sym ;

    s_map->insert({ sym, fresh });
    s_seq->push_back(fresh);

    return fresh;
}

static class_meta *get_class(symbol s) { return get<class_meta>(s, type_category::is_class); }
static enum_meta  *get_enum (symbol s) { return get<enum_meta >(s, type_category::is_enum ); }

void commit_meta(type_meta *meta) {
    switch (meta->type) {
    case type_category::is_const:
        {
            auto real = (const_meta *)meta;
            if (real->belong_class) {
                //is class member.
                class_meta *belong = get_class(real->belong_class);
                belong->const_map.insert({ real->sym, real });
                belong->const_seq.push_back(real);

            } else if (real->belong_enum) {
                //is enum value.
                enum_meta *belong = get_enum(real->belong_enum);
                belong->value_map.insert({ real->sym, real });
                belong->value_seq.push_back(real);

            } else {
                //is global constant.
                s_map->insert({ real->sym, real });
                s_seq->push_back(real);
            }
        }
        break;

    case type_category::is_enum:
        {
            get_enum(meta->sym);
        }
        break;

    case type_category::is_function:
        {
            auto real = (function_meta *)meta;
            if (real->belong_class) {
                //is class function.
                class_meta *belong = get_class(real->belong_class);
                belong->function_map.insert({ real->sym, real });
                belong->function_seq.push_back(real);

            } else {
                //is global function.
                s_map->insert({ real->sym, real });
                s_seq->push_back(real);
            }
        }
        break;

    case type_category::is_class:
        {
            get_class(meta->sym);
        }
        break;

    default:
        {
            s_map->insert({ meta->sym, meta });
            s_seq->push_back(meta);
        }
    }
}

const type_meta *find_type(const char *name) {
    auto it = s_map->find(symbol::find(name));
    if (it != s_map->end()) {
        return it->second;
    }
    return nullptr;
}

const std::vector<type_meta *> &all_types() {
    return *s_seq;
}

} //end reflect.
