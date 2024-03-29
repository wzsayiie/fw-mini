#include "rmetajson.h"
#include "djson.h"
#include "rmeta.h"

using dash::json;
using dash::json_object;

using std::map;
using std::string;
using std::vector;

struct fcndesc_node : json_object {
    d_json_object(fcndesc_node)

    json<string> type {"type"};
    json<string> note {"note"};
};

struct typedesc_node : json_object {
    d_json_object(typedesc_node)

    json<string> type {"type"};
    json<string> qual {"qual"};
};

struct class_node : json_object {
    d_json_object(class_node)

    json<string>                    base_type        {"base_type"       };
    json<bool>                      abstracted       {"abstracted"      };
    json<map<string, string>>       static_strings   {"static_strings"  };
    json<map<string, double>>       static_numbers   {"static_numbers"  };
    json<map<string, fcndesc_node>> static_functions {"static_functions"};
    json<map<string, fcndesc_node>> inst_functions   {"inst_functions"  };
};

struct function_node : json_object {
    d_json_object(function_node)

    json<vector<typedesc_node>> args {"args"};
    json<typedesc_node>         ret  {"ret" };
};

struct enum_node : json_object {
    d_json_object(enum_node)

    json<map<string, double>> values {"values"};
};

struct map_node : json_object {
    d_json_object(map_node)

    json<typedesc_node> key {"key"};
    json<typedesc_node> val {"val"};
};

struct vector_node : json_object {
    d_json_object(vector_node)

    json<typedesc_node> val {"val"};
};

struct set_node : json_object {
    d_json_object(set_node)

    json<typedesc_node> val {"val"};
};

struct root_node : json_object {
    d_json_object(root_node)

    json<map<string, class_node>>    class_infos    {"class_infos"   };
    json<map<string, function_node>> function_infos {"function_infos"};
    json<map<string, enum_node>>     enum_infos     {"enum_infos"    };
    json<map<string, map_node>>      map_infos      {"map_infos"     };
    json<map<string, vector_node>>   vector_infos   {"vector_infos"  };
    json<map<string, set_node>>      set_infos      {"set_infos"     };

    json<map<string, string>>       strings   {"strings"  };
    json<map<string, double>>       numbers   {"numbers"  };
    json<map<string, fcndesc_node>> functions {"functions"};
    json<vector<string>>            classes   {"classes"  };
    json<vector<string>>            enums     {"enums"    };
};

static string qualifer_string_of(reflect::qualifier qual) {
    switch (qual) {
        case reflect::qualifier::value       : return "value"       ;
        case reflect::qualifier::const_ptr   : return "const_ptr"   ;
        case reflect::qualifier::ptr         : return "ptr"         ;
        case reflect::qualifier::const_ref   : return "const_ref"   ;
        case reflect::qualifier::ref         : return "ref"         ;
        case reflect::qualifier::const_shared: return "const_shared";
        case reflect::qualifier::shared      : return "shared"      ;

        default: return "";
    }
}

static void insert_class_info(root_node *root, const string &name, reflect::class_meta *meta) {
    class_node info;

    info.base_type  = meta->base_type.str();
    info.abstracted = meta->abstract;

    for (auto &pair : meta->static_variables) {
        const reflect::any &value = pair.second.value;

        if (value.preferred_type() == reflect::data_type::is_string) {
            info.static_strings[pair.first] = value.as_string();
        } else {
            info.static_numbers[pair.first] = value.as_double();
        }
    }
    
    for (auto &pair : meta->static_functions) {
        fcndesc_node desc;
        desc.type = pair.second.type.str();
        desc.note = pair.second.note;

        info.static_functions[pair.first] = desc;
    }

    for (auto &pair : meta->inst_functions) {
        fcndesc_node desc;
        desc.type = pair.second.type.str();
        desc.note = pair.second.note;

        info.inst_functions[pair.first] = desc;
    }

    root->class_infos[name] = info;
}

static void insert_function_info(root_node *root, const string &name, reflect::function_meta *meta) {
    function_node info;

    for (size_t i = 0; i < meta->arg_types.size(); ++i) {
        typedesc_node desc;
        desc.type = meta->arg_types[i].str();
        desc.qual = qualifer_string_of(meta->arg_quals[i]);

        info.args.push_back(desc);
    }

    info.ret.type = meta->ret_type.str();
    info.ret.qual = qualifer_string_of(meta->ret_qual);

    root->function_infos[name] = info;
}

static void insert_enum_info(root_node *root, const string &name, reflect::enum_meta *meta) {
    enum_node info;
    
    for (auto &pair : meta->values) {
        info.values[pair.first] = pair.second.value.as_double();
    }

    root->enum_infos[name] = info;
}

static void insert_map_info(root_node *root, const string &name, reflect::map_meta *meta) {
    map_node info;

    info.key.type = meta->key_type  .str();
    info.val.type = meta->value_type.str();
    info.key.qual = qualifer_string_of(meta->key_qual  );
    info.val.qual = qualifer_string_of(meta->value_qual);

    root->map_infos[name] = info;
}

static void insert_vector_info(root_node *root, const string &name, reflect::vector_meta *meta) {
    vector_node info;

    info.val.type = meta->value_type.str();
    info.val.qual = qualifer_string_of(meta->value_qual);

    root->vector_infos[name] = info;
}

static void insert_set_info(root_node *root, const string &name, reflect::set_meta *meta) {
    set_node info;

    info.val.type = meta->value_type.str();
    info.val.qual = qualifer_string_of(meta->value_qual);

    root->set_infos[name] = info;
}

string reflect::meta_json_description() {
    root_node root;

    //type infos.
    for (auto &pair : *type_metas()) {
        std::string t = pair.first.str();
        type_meta  *m = pair.second;

        switch (m->cate) {
            case category::is_class   : insert_class_info   (&root, t, (class_meta    *)m); break;
            case category::is_function: insert_function_info(&root, t, (function_meta *)m); break;
            case category::is_enum    : insert_enum_info    (&root, t, (enum_meta     *)m); break;
            case category::is_map     : insert_map_info     (&root, t, (map_meta      *)m); break;
            case category::is_vector  : insert_vector_info  (&root, t, (vector_meta   *)m); break;
            case category::is_set     : insert_set_info     (&root, t, (set_meta      *)m); break;

            default:;
        }
    }

    //global constant variables.
    for (auto &pair : *variables()) {
        const any &value = pair.second.value;

        if (value.preferred_type() == reflect::data_type::is_string) {
            root.strings[pair.first] = value.as_string();
        } else {
            root.numbers[pair.first] = value.as_double();
        }
    }

    //global functions.
    for (auto &pair : *functions()) {
        fcndesc_node desc;
        desc.type = pair.second.type.str();
        desc.note = pair.second.note;

        root.functions[pair.first] = desc;
    }

    //classes.
    for (auto &it : *classes()) {
        root.classes.push_back(it.second.type.str());
    }

    //enums.
    for (auto &it : *enums()) {
        root.enums.push_back(it.second.type.str());
    }

    return root.encode();
}
