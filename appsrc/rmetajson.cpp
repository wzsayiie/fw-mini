#include "rmetajson.h"
#include "rmeta.h"

using dash::json;
using dash::json_object;

using std::map;
using std::string;
using std::vector;

struct fnote_node : json_object {
    d_json_object(fnote_node)

    json<string> type {"type"};
    json<string> note {"note"};
};

struct class_node : json_object {
    d_json_object(class_node)

    json<string>                  base_type        {"base_type"       };
    json<bool>                    abstracted       {"abstracted"      };
    json<map<string, string>>     static_strings   {"static_strings"  };
    json<map<string, double>>     static_numbers   {"static_numbers"  };
    json<map<string, fnote_node>> static_functions {"static_functions"};
    json<map<string, fnote_node>> inst_functions   {"inst_functions"  };
};

struct function_node : json_object {
    d_json_object(function_node)

    json<vector<string>> arg_qualifiers {"arg_qualifiers"};
    json<vector<string>> arg_types      {"arg_types"     };
    json<string>         ret_qualifier  {"ret_qualifier" };
    json<string>         ret_type       {"ret_type"      };
};

struct enum_node : json_object {
    d_json_object(enum_node)

    json<map<string, double>> values {"values"};
};

struct map_node : json_object {
    d_json_object(map_node)

    json<string> key_type {"key_type"};
    json<string> val_type {"val_type"};
};

struct vector_node : json_object {
    d_json_object(vector_node)

    json<string> val_type {"val_type"};
};

struct set_node : json_object {
    d_json_object(set_node)

    json<string> val_type {"val_type"};
};

struct root_node : json_object {
    d_json_object(root_node)

    json<map<string, class_node>>    class_infos    {"class_infos"   };
    json<map<string, function_node>> function_infos {"function_infos"};
    json<map<string, enum_node>>     enum_infos     {"enum_infos"    };
    json<map<string, map_node>>      map_infos      {"map_infos"     };
    json<map<string, vector_node>>   vector_infos   {"vector_infos"  };
    json<map<string, set_node>>      set_infos      {"set_infos"     };

    json<map<string, string>>     strings   {"strings"  };
    json<map<string, double>>     numbers   {"numbers"  };
    json<map<string, fnote_node>> functions {"functions"};
    json<map<string, string>>     classes   {"classes"  };
    json<map<string, string>>     enums     {"enums"    };
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
            root->strings[pair.first] = value.as_string();
        } else {
            root->numbers[pair.first] = value.as_double();
        }
    }
    
    for (auto &pair : meta->static_functions) {
        fnote_node fnote;
        fnote.type = pair.second.type.str();
        fnote.note = pair.second.note;

        info.static_functions[pair.first] = fnote;
    }

    for (auto &pair : meta->inst_functions) {
        fnote_node fnote;
        fnote.type = pair.second.type.str();
        fnote.note = pair.second.note;

        info.inst_functions[pair.first] = fnote;
    }

    root->class_infos[name] = info;
}

static void insert_function_info(root_node *root, const string &name, reflect::function_meta *meta) {
    function_node info;
    
    for (auto &it : meta->arg_quals) {
        info.arg_qualifiers.push_back(qualifer_string_of(it));
    }
    for (auto &it : meta->arg_types) {
        info.arg_types.push_back(it.str());
    }

    info.ret_qualifier = qualifer_string_of(meta->ret_qual);
    info.ret_type = meta->ret_type.str();

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

    info.key_type = meta->key_type  .str();
    info.val_type = meta->value_type.str();

    root->map_infos[name] = info;
}

static void insert_vector_info(root_node *root, const string &name, reflect::vector_meta *meta) {
    vector_node info;
    info.val_type = meta->value_type.str();
    root->vector_infos[name] = info;
}

static void insert_set_info(root_node *root, const string &name, reflect::set_meta *meta) {
    set_node info;
    info.val_type = meta->value_type.str();
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
        fnote_node fnote;
        fnote.type = pair.second.type.str();
        fnote.note = pair.second.note;

        root.functions[pair.first] = fnote;
    }

    //classes.
    for (auto &it : *classes()) {
        root.classes[it.first] = it.second.type.str();
    }

    //enums.
    for (auto &it : *enums()) {
        root.enums[it.first] = it.second.type.str();
    }

    return root.encode();
}
