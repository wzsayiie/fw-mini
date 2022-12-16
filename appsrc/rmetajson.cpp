#include "rmetajson.h"
#include "rmeta.h"

using dash::json;
using dash::json_object;

using std::map;
using std::string;
using std::vector;

struct variable_node : json_object {
    d_json_object(variable_node)

    json<string> type      {"type"     };
    json<double> num_value {"num_value"};
    json<string> str_value {"str_value"};
};

struct class_node : json_object {
    d_json_object(class_node)

    json<string>                     base_type        {"base_type"       };
    json<bool>                       abstracted       {"abstracted"      };
    json<map<string, variable_node>> static_variables {"static_variables"};
    json<map<string, string>>        static_functions {"static_functions"};
    json<map<string, string>>        inst_functions   {"inst_functions"  };
};

struct function_node : json_object {
    d_json_object(function_node)

    json<vector<string>> arg_qualifiers {"arg_qualifiers"};
    json<vector<string>> arg_types      {"arg_types"     };
    json<string>         ret_qualifier  {"ret_qualifier" };
    json<string>         ret_type       {"ret_type"      };
    json<string>         note           {"note"          };
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

struct type_node : json_object {
    d_json_object(type_node)

    json<string>     catagory      {"catagory"     };
    json<class_node> class_info    {"class_info"   };
    json<class_node> function_info {"function_info"};
    json<class_node> enum_info     {"enum_info"    };
    json<class_node> map_info      {"map_info"     };
    json<class_node> vector_info   {"vector_info"  };
    json<class_node> set_info      {"set_info"     };
};

struct root_node : json_object {
    d_json_object(root_node)

    json<map<string, type_node>>     types     {"types"    };
    json<map<string, variable_node>> variables {"variables"};
    json<map<string, string>>        functions {"functions"};
    json<map<string, string>>        classes   {"classes"  };
    json<map<string, string>>        enums     {"enums"    };
};

string reflect::meta_json_description() {
    root_node root;

    //global constant variables.
    for (auto &it : *variables()) {
        variable_node node; {
            node.type      = it.second.type.str();
            node.num_value = it.second.value.as_double();
            node.str_value = it.second.value.as_string();
        }
        root.variables[it.first] = node;
    }

    //global functions.
    for (auto &it : *functions()) {
        root.functions[it.first] = it.second.type.str();
    }

    //classes.
    for (auto &it : *classes()) {
        root.classes[it.first] = it.second.type.str();
    }

    //enums.
    for (auto &it : *enums()) {
        root.enums[it.first] = it.second.type.str();
    }

    printf("%s", root.encode().c_str());
    return root.encode();
}
