#pragma once

#include "dencodable.h"

namespace dash {

//context:

struct json_encoding_ctx {
    std::string buffer;
    
    bool indent_head = false;
    int  indent_deep = 0;
    
    //if a "boolean" or "number" is used as the key of "object",
    //it needs to be represented as a string type in json.
    bool this_is_key = false;
};

struct json_parsing_ctx {
    const char *ptr = nullptr;
    const char *end = nullptr;
    
    //the value to be parsed.
    const char *handle = nullptr;
    const char *intent = nullptr;
    
    bool this_is_key = false;
};

//field & object:

template<class> struct is_valid_json_type;

template<class> struct json_encoder;
template<class> struct json_parser ;
template<class> struct json_cleaner;

template<class Type> class json : public encodable<Type> {
    static_assert(is_valid_json_type<Type>::value);
    
public:
    typedef encodable<Type> _inheritable;

    using _inheritable::_inheritable;
    using _inheritable::operator=;

public:
    void on_encode(void *context) const override {
        json_encoder<Type>::encode((json_encoding_ctx *)context, *this);
    }

    void on_parse(void *context) override {
        json_parser<Type>::parse((json_parsing_ctx *)context, this);
    }
    
    void on_clear() override {
        json_cleaner<Type>::clear(this);
    }
};

struct d_exportable json_object : encodable_object {
    std::string encode() const;
    bool parse(const std::string &text);
    void clear();
};

//type constraints:

template<class T> struct is_valid_json_type {
    static const bool value =
        std::is_base_of<json_object, T>::value ||
        std::is_same   <std::string, T>::value ||
        std::is_same   <double     , T>::value ||
        std::is_same   <bool       , T>::value ;
};

template<class K, class V> struct is_valid_json_type<std::map<K, V>> {
    static const bool value =
        (
            std::is_same<std::string, K>::value ||
            std::is_same<double     , K>::value ||
            std::is_same<bool       , K>::value
        )
        &&
        (
            is_valid_json_type<V>::value
        );
};

template<class V> struct is_valid_json_type<std::vector<V>> {
    static const bool value = is_valid_json_type<V>::value;
};

//encoder:

d_exportable void json_encode_indent (json_encoding_ctx *ctx, bool needed);
d_exportable void json_encode_endline(json_encoding_ctx *ctx);
d_exportable void json_encode_space  (json_encoding_ctx *ctx);
d_exportable void json_encode_string (json_encoding_ctx *ctx, const std::string &value);
d_exportable void json_encode_double (json_encoding_ctx *ctx, double             value);
d_exportable void json_encode_bool   (json_encoding_ctx *ctx, bool               value);

template<class Key, class Value> struct json_encoder<std::map<Key, Value>> {
    static void encode(json_encoding_ctx *ctx, const std::map<Key, Value> &map) {
        //"{".
        json_encode_indent(ctx, ctx->indent_head);
        ctx->buffer.append("{");
        json_encode_endline(ctx);

        size_t count = 0;
        for (auto &[key, value] : map) {
            //key.
            ctx->this_is_key = true;
            ctx->indent_head = true;
            ctx->indent_deep += 1;
            json_encoder<Key>::encode(ctx, key);
            ctx->indent_deep -= 1;
            
            //":".
            ctx->buffer.append(":");
            json_encode_space(ctx);
            
            //value.
            ctx->this_is_key = false;
            ctx->indent_head = false;
            ctx->indent_deep += 1;
            json_encoder<Value>::encode(ctx, value);
            ctx->indent_deep -= 1;
            
            //",".
            if (++count < map.size()) {
                ctx->buffer.append(",");
            }
            json_encode_endline(ctx);
        }

        //"}".
        json_encode_indent(ctx, true);
        ctx->buffer.append("}");
    }
};

template<class Value> struct json_encoder<std::vector<Value>> {
    static void encode(json_encoding_ctx *ctx, const std::vector<Value> &vector) {
        //"[".
        json_encode_indent(ctx, ctx->indent_head);
        ctx->buffer.append("[");
        json_encode_endline(ctx);
        
        size_t count = 0;
        for (auto &value : vector) {
            //value.
            ctx->indent_head = true;
            ctx->indent_deep += 1;
            json_encoder<Value>::encode(ctx, value);
            ctx->indent_deep -= 1;
            
            //",".
            if (++count < vector.size()) {
                ctx->buffer.append(",");
            }
            json_encode_endline(ctx);
        }
        
        //"]".
        json_encode_indent(ctx, true);
        ctx->buffer.append("]");
    }
};

template<class Object> struct json_encoder {
    static void encode(json_encoding_ctx *ctx, const Object &obj) {
        json_encoder<std::map<std::string, encodable_field *>>::encode(ctx, obj);
    }
};

template<> struct json_encoder<encodable_field *> {
    static void encode(json_encoding_ctx *ctx, encodable_field *field) {
        field->on_encode(ctx);
    }
};

template<> struct json_encoder<std::string> {
    static void encode(json_encoding_ctx *ctx, const std::string &value) {
        json_encode_string(ctx, value);
    }
};

template<> struct json_encoder<double> {
    static void encode(json_encoding_ctx *ctx, double value) {
        json_encode_double(ctx, value);
    }
};

template<> struct json_encoder<bool> {
    static void encode(json_encoding_ctx *ctx, bool value) {
        json_encode_bool(ctx, value);
    }
};

//paser:

class json_ignore {};

d_exportable void json_set_intent(json_parsing_ctx *ctx, const char *intent);
d_exportable void json_skip_blank(json_parsing_ctx *ctx);
d_exportable char json_will_read (json_parsing_ctx *ctx);

[[noreturn]] d_exportable void json_exception(json_parsing_ctx *ctx) noexcept(false);

d_exportable void json_parse_token (json_parsing_ctx *ctx, const char *target) noexcept(false);
d_exportable void json_parse_ignore(json_parsing_ctx *ctx, json_ignore *outer) noexcept(false);
d_exportable void json_parse_string(json_parsing_ctx *ctx, std::string *outer) noexcept(false);
d_exportable void json_parse_double(json_parsing_ctx *ctx, double      *outer) noexcept(false);
d_exportable void json_parse_bool  (json_parsing_ctx *ctx, bool        *outer) noexcept(false);

template<> struct json_parser<json_ignore> {
    static void parse(json_parsing_ctx *ctx, json_ignore *value) {
        json_parse_ignore(ctx, value);
    }
};

template<class Key, class Value> struct json_map_value_parser {
    static void parse(
        std::map<Key, Value> *map, const Key &key, json_parsing_ctx *ctx) noexcept(false)
    {
        Value value{};
        json_parser<Value>::parse(ctx, &value);

        if (map) {
            map->insert({ key, value });
        }
    }
};

template<> struct json_map_value_parser<std::string, encodable_field *> {
    static void parse(
        std::map<std::string, encodable_field *> *map,
        const std::string &key,
        json_parsing_ctx  *ctx) noexcept(false)
    {
        //the key exists, insert the value.
        auto it = map->find(key);
        if (it != map->end()) {
            it->second->on_parse(ctx);
            return;
        }
        
        //ignore the value.
        json_parser<json_ignore>::parse(ctx, nullptr);
    }
};

template<class Key, class Value> struct json_parser<std::map<Key, Value>> {
    static void parse(json_parsing_ctx *ctx, std::map<Key, Value> *map) noexcept(false) {
        json_set_intent(ctx, "object");
        
        //if is a "null".
        if (json_will_read(ctx) == 'n') {
            json_parse_token(ctx, "null");
            return;
        }

        json_parse_token(ctx, "{");
        
        //if is a "{}".
        if (json_will_read(ctx) == '}') {
            json_parse_token(ctx, "}");
            return;
        }
        
        while (true) {
            //key.
            ctx->this_is_key = true;
            Key key{};
            json_parser<Key>::parse(ctx, &key);
            
            //":".
            json_set_intent (ctx, "\":\"");
            json_parse_token(ctx, ":");
            
            //value.
            ctx->this_is_key = false;
            json_map_value_parser<Key, Value>::parse(map, key, ctx);
            
            //"}" or ",".
            if (json_will_read(ctx) == '}') {
                json_parse_token(ctx, "}");
                break;
            }
            json_set_intent (ctx, "\",\"");
            json_parse_token(ctx, ",");
        }
        return;
    }
};

template<class Value> struct json_parser<std::vector<Value>> {
    static void parse(json_parsing_ctx *ctx, std::vector<Value> *vector) noexcept(false) {
        json_set_intent(ctx, "array");
        
        //if is a "null".
        if (json_will_read(ctx) == 'n') {
            json_parse_token(ctx, "null");
            return;
        }

        json_parse_token(ctx, "[");
        
        //if is a "{}".
        if (json_will_read(ctx) == ']') {
            json_parse_token(ctx, "]");
            return;
        }
        
        while (true) {
            //value.
            Value value{};
            json_parser<Value>::parse(ctx, &value);
            if (vector) {
                vector->push_back(value);
            }
            
            //"}" or ",".
            if (json_will_read(ctx) == ']') {
                json_parse_token(ctx, "]");
                break;
            }
            json_set_intent (ctx, "\",\"");
            json_parse_token(ctx, ",");
        }
    }
};

template<class Object> struct json_parser {
    static void parse(json_parsing_ctx *ctx, Object *obj) noexcept(false) {
        json_parser<std::map<std::string, encodable_field *>>::parse(ctx, obj);
    }
};

template<> struct json_parser<std::string> {
    static void parse(json_parsing_ctx *ctx, std::string *value) noexcept(false) {
        json_parse_string(ctx, value);
    }
};

template<> struct json_parser<double> {
    static void parse(json_parsing_ctx *ctx, json<double> *field) noexcept(false) {
        double value = 0;
        json_parse_double(ctx, &value);
        *field = value;
    }
    
    //"json<double> *" can not be converted to "double *",
    //need to rewrite this function of the same name.
    static void parse(json_parsing_ctx *ctx, double *value) noexcept(false) {
        json_parse_double(ctx, value);
    }
};

template<> struct json_parser<bool> {
    static void parse(json_parsing_ctx *ctx, json<bool> *field) noexcept(false) {
        bool value = false;
        json_parse_bool(ctx, &value);
        *field = value;
    }
    
    static void parse(json_parsing_ctx *ctx, bool *value) noexcept(false) {
        json_parse_bool(ctx, value);
    }
};

//cleaner:

template<class Key, class Value> struct json_cleaner<std::map<Key, Value>> {
    static void clear(json<std::map<Key, Value>> *field) {
        field->clear();
    }
};

template<class Value> struct json_cleaner<std::vector<Value>> {
    static void clear(json<std::vector<Value>> *field) {
        field->clear();
    }
};

template<class Object> struct json_cleaner {
    static void clear(json<Object> *field) {
        field->clear();
    }
};

template<> struct json_cleaner<std::string> {
    static void clear(json<std::string> *field) {
        field->clear();
    }
};

template<> struct json_cleaner<double> {
    static void clear(json<double> *field) {
        *field = 0;
    }
};

template<> struct json_cleaner<bool> {
    static void clear(json<bool> *field) {
        *field = false;
    }
};

} //end dash.

#define d_json_object(cls) d_encodable_object(cls)
