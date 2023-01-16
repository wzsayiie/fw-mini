#include "rmeta.h"
#include "dlazy.h"

namespace reflect {

static dash::lazy<std::map<symbol, type_meta *>> _type_metas;

static dash::lazy<std::map<std::string, variable>> _variables;
static dash::lazy<std::map<std::string, variable>> _functions;
static dash::lazy<std::map<std::string, variable>> _classes  ;
static dash::lazy<std::map<std::string, variable>> _enums    ;

static type_meta *set_type_meta(const symbol &sym, category cate) {
    type_meta *meta;
    switch (cate) {
        case category::is_function: meta = new function_meta(); break;
        case category::is_vector  : meta = new vector_meta  (); break;
        case category::is_map     : meta = new map_meta     (); break;
        case category::is_set     : meta = new set_meta     (); break;
        case category::is_class   : meta = new class_meta   (); break;
        case category::is_enum    : meta = new enum_meta    (); break;
        default /* basic type */  : meta = new type_meta    (); break;
    }

    meta->cate = cate;
    (*_type_metas)[sym] = meta;
    return meta;
}

type_meta *commit_type_meta(const symbol &sym, category cate) {
    if (!query_type_meta(sym)) {
        return set_type_meta(sym, cate);
    }
    return nullptr;
}

void commit_variable(const std::string &name, const symbol &type, const any &value) {
    variable var = { type, value };
    (*_variables)[name] = var;
}

void commit_function(const std::string &name, const symbol &type, const char *note, const any &value) {
    variable var = { type, value, (note ? note : "") };
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

void commit_class_static_variable(
    const symbol &cls, const std::string &name, const symbol &type, const any &value)
{
    auto meta = (class_meta *)query_type_meta(cls, category::is_class);
    if (meta) {
        variable var = { type, value };
        meta->static_variables[name] = var;
    }
}

void commit_class_static_function(
    const symbol &cls, const std::string &name, const symbol &type, const char *note, const any &value)
{
    auto meta = (class_meta *)query_type_meta(cls, category::is_class);
    if (meta) {
        variable var = { type, value, (note ? note : "") };
        meta->static_functions[name] = var;
    }
}

void commit_class_inst_function(
    const symbol &cls, const std::string &name, const symbol &type, const char *note, const any &value)
{
    auto meta = (class_meta *)query_type_meta(cls, category::is_class);
    if (meta) {
        variable var = { type, value, (note ? note : "") };
        meta->inst_functions[name] = var;
    }
}

void commit_enum_value(
    const symbol &enu, const std::string &name, const symbol &type, const any &value)
{
    auto meta = (enum_meta *)query_type_meta(enu, category::is_enum);
    if (meta) {
        variable var = { type, value };
        meta->values[name] = var;
    }
}

const std::map<symbol, type_meta *> *type_metas() {
    return &_type_metas;
}

const type_meta *query_type_meta(const symbol &sym, category cate) {
    const type_meta *meta = query_type_meta(sym);
    if (meta && meta->cate == cate) {
        return meta;
    }

    return nullptr;
}

const type_meta *query_type_meta(const symbol &sym) {
    static bool to_initialize = true;
    if (to_initialize) {
        set_type_meta(symbol_of<any        >::value(), category::is_any   );
        set_type_meta(symbol_of<void       >::value(), category::is_void  );
        set_type_meta(symbol_of<bool       >::value(), category::is_bool  );
        set_type_meta(symbol_of<char       >::value(), category::is_char  );
        set_type_meta(symbol_of<uint8_t    >::value(), category::is_byte  );
        set_type_meta(symbol_of<int        >::value(), category::is_int   );
        set_type_meta(symbol_of<int64_t    >::value(), category::is_int64 );
        set_type_meta(symbol_of<float      >::value(), category::is_float );
        set_type_meta(symbol_of<double     >::value(), category::is_double);
        set_type_meta(symbol_of<std::string>::value(), category::is_string);
        
        to_initialize = false;
    }
    
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
