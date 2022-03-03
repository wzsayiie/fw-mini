#include "rmeta.h"

namespace reflect {

static dash::lazy_var<std::map<symbol *, type_meta *>> s_map;
static dash::lazy_var<std::vector<type_meta *>>        s_seq;

template<class Meta> Meta *get(symbol *name, type_category type) {
    auto it = s_map->find(name);
    if (it != s_map->end()) {
        return (Meta *)it->second;
    }

    auto fresh = new Meta();
    fresh->type = type;
    fresh->name = name;

    s_map->insert({ name, fresh });
    s_seq->push_back(fresh);

    return fresh;
}

static class_meta *get_class(symbol *n) { return get<class_meta>(n, type_category::is_class); }
static enum_meta  *get_enum (symbol *n) { return get<enum_meta >(n, type_category::is_enum ); }

void commit_meta(type_meta *meta) {
    switch (meta->type) {
    case type_category::is_function:
        {
            auto real = (function_meta *)meta;
            if (real->belong_class) {
                //is class function.
                class_meta *belong = get_class(real->belong_class);
                belong->function_map.insert({ real->name, real });
                belong->function_seq.push_back(real);

            } else {
                //is global function.
                s_map->insert({ real->name, real });
                s_seq->push_back(real);
            }
        }
        break;

    case type_category::is_class:
        {
            get_class(meta->name);
        }
        break;

    case type_category::is_const:
        {
            auto real = (const_meta *)meta;
            if (real->belong_enum) {
                //is enum value.
                enum_meta *belong = get_enum(real->belong_enum);
                belong->value_map.insert({ real->name, real });
                belong->value_seq.push_back(real);

            } else {
                //is global constant.
                s_map->insert({ real->name, real });
                s_seq->push_back(real);
            }
        }
        break;

    case type_category::is_enum:
        {
            get_enum(meta->name);
        }
        break;

    default:
        {
            s_map->insert({ meta->name, meta });
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

} // end reflect.
