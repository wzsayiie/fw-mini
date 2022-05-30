#include "rmeta.h"

namespace reflect {

static dash::lazy<std::map<symbol, type_meta *>> _type_metas;

static dash::lazy<std::map<std::string, variable>> _variables;
static dash::lazy<std::map<std::string, variable>> _functions;
static dash::lazy<std::map<std::string, variable>> _classes  ;
static dash::lazy<std::map<std::string, variable>> _enums    ;

void commit_type_meta(const symbol &sym, type_meta *meta) {
    if (_type_metas->find(sym) == _type_metas->end()) {
        _type_metas->insert({ sym, meta });
    } else {
        delete meta;
    }
}

void commit_variable(const std::string &n, const variable &v) { _variables->insert({ n, v }); }
void commit_function(const std::string &n, const variable &v) { _functions->insert({ n, v }); }

void commit_class_variable(const symbol &cls, const std::string &name, const variable &value) {
    auto meta = (class_meta *)query_type_meta(cls);
    if (meta && meta->category == type_category::is_class) {
        meta->cls_variables[name] = value;
    }
}

void commit_class_function(const symbol &cls, const std::string &name, const variable &value) {
    auto meta = (class_meta *)query_type_meta(cls);
    if (meta && meta->category == type_category::is_class) {
        meta->cls_functions[name] = value;
    }
}

void commit_object_function(const symbol &cls, const std::string &name, const variable &value) {
    auto meta = (class_meta *)query_type_meta(cls);
    if (meta && meta->category == type_category::is_class) {
        meta->obj_functions[name] = value;
    }
}

void commit_enum_value(const symbol &enu, const std::string &name, const variable &value) {
    auto meta = (enum_meta *)query_type_meta(enu);
    if (meta && meta->category == type_category::is_enum) {
        meta->values[name] = value;
    }
}

const type_meta *query_type_meta(const symbol &sym) {
    auto it = _type_metas->find(sym);
    if (it != _type_metas->end()) {
        return it->second;
    }

    return nullptr;
}

const std::map<std::string, variable> *variables() { return &_variables; }
const std::map<std::string, variable> *functions() { return &_functions; }
const std::map<std::string, variable> *classes  () { return &_classes  ; }
const std::map<std::string, variable> *enums    () { return &_enums    ; }

} //end reflect.
