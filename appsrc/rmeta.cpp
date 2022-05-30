#include "rmeta.h"

namespace reflect {

static dash::lazy<std::map<symbol, type_meta *>> _type_metas;

static dash::lazy<std::map<std::string, variable>> _variables;
static dash::lazy<std::map<std::string, variable>> _functions;
static dash::lazy<std::map<std::string, variable>> _classes  ;
static dash::lazy<std::map<std::string, variable>> _enums    ;

type_meta *commit_type_meta(const symbol &sym, type_category category) {
    if (_type_metas->find(sym) != _type_metas->end()) {
        return nullptr;
    }

    type_meta *meta;
    switch (category) {
        case type_category::is_function: meta = new function_meta(); break;
        case type_category::is_vector  : meta = new vector_meta  (); break;
        case type_category::is_map     : meta = new map_meta     (); break;
        case type_category::is_set     : meta = new set_meta     (); break;
        case type_category::is_class   : meta = new class_meta   (); break;
        case type_category::is_enum    : meta = new enum_meta    (); break;
        default /* basic simple type */: meta = new type_meta    (); break;
    }

    meta->category = category;
    (*_type_metas)[sym] = meta;
    return meta;
}

void commit_variable(const std::string &name, const symbol &type, const any &value) {
    variable var = { type, value };
    (*_variables)[name] = var;
}

void commit_function(const std::string &name, const symbol &type, const any &value) {
    variable var = { type, value };
    (*_functions)[name] = var;
}

void commit_class(const std::string &name, const symbol &type) {
    variable var = { type, 0 };
    (*_classes)[name] = var;
}

void commit_enum(const std::string &name, const symbol &type) {
    variable var = { type, 0 };
    (*_enums)[name] = var;
}

void commit_class_variable(const symbol &cls, const std::string &name, const symbol &type, const any &value) {
    auto meta = (class_meta *)query_type_meta(cls, type_category::is_class);
    if (meta) {
        variable var = { type, value };
        meta->cls_variables[name] = var;
    }
}

void commit_class_function(const symbol &cls, const std::string &name, const symbol &type, const any &value) {
    auto meta = (class_meta *)query_type_meta(cls, type_category::is_class);
    if (meta) {
        variable var = { type, value };
        meta->cls_functions[name] = var;
    }
}

void commit_object_function(const symbol &cls, const std::string &name, const symbol &type, const any &value) {
    auto meta = (class_meta *)query_type_meta(cls, type_category::is_class);
    if (meta) {
        variable var = { type, value };
        meta->obj_functions[name] = var;
    }
}

void commit_enum_value(const symbol &enu, const std::string &name, const symbol &type, const any &value) {
    auto meta = (enum_meta *)query_type_meta(enu, type_category::is_enum);
    if (meta) {
        variable var = { type, value };
        meta->values[name] = var;
    }
}

const type_meta *query_type_meta(const symbol &sym, type_category category) {
    const type_meta *meta = query_type_meta(sym);
    if (meta && meta->category == category) {
        return meta;
    }

    return nullptr;
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
