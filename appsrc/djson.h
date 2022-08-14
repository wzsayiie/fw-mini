#pragma once

#include "dencodable.h"

namespace dash {

//contexet:

struct json_encoding_ctx {
    std::string buffer;
    
    bool indent_head = false;
    int  indent_deep = 0;
    
    //if a "boolean" or "number" is used as the key of "object",
    //it needs to be represented as a string type in json.
    bool to_string = false;
};

struct json_parsing_ctx {
    const char *ptr  = nullptr;
    const char *end  = nullptr;
    bool from_string = false;
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

class d_exportable json_object : public encodable_object {
public:
    std::string encode() const;
    bool parse(const std::string &text);
    void clear();

private: public:
    void encode_with_ctx(json_encoding_ctx *ctx) const;
    void parse_with_ctx (json_parsing_ctx  *ctx);
};

//type constraints:

template<class T> struct is_valid_json_type {
    static const bool value = (
        std::is_base_of<json_object, T>::value ||
        std::is_same   <std::string, T>::value ||
        std::is_same   <double     , T>::value ||
        std::is_same   <bool       , T>::value
    );
};

template<class K, class V> struct is_valid_json_type<std::map<K, V>> {
    static const bool value = (
        (std::is_same<std::string, K>::value || std::is_same<double, K>::value) &&
        (is_valid_json_type<V>::value)
    );
};

template<class V> struct is_valid_json_type<std::vector<V>> {
    static const bool value = is_valid_json_type<V>::value;
};

//encoder:

d_exportable void json_encode_indent (json_encoding_ctx *ctx, bool need);
d_exportable void json_encode_endline(json_encoding_ctx *ctx);
d_exportable void json_encode_space  (json_encoding_ctx *ctx);
d_exportable void json_encode_string (json_encoding_ctx *ctx, const std::string &value);
d_exportable void json_encode_double (json_encoding_ctx *ctx, double             value);
d_exportable void json_encode_bool   (json_encoding_ctx *ctx, bool               value);

template<class Key, class Value> struct json_encoder<std::map<Key, Value>> {
    static void encode(json_encoding_ctx *ctx, const std::map<Key, Value> &field) {
        json_encode_indent(ctx, ctx->indent_head);
        ctx->buffer.append("{");
        json_encode_endline(ctx);

        size_t count = 0;
        for (auto &pair : field) {
            //key.
            ctx->to_string   = true;
            ctx->indent_head = true;
            ctx->indent_deep += 1;
            json_encoder<Key>::encode(ctx, pair.first);
            ctx->indent_deep -= 1;
            
            //":".
            ctx->buffer.append(":");
            json_encode_space(ctx);
            
            //value.
            ctx->to_string   = false;
            ctx->indent_head = false;
            ctx->indent_deep += 1;
            json_encoder<Value>::encode(ctx, pair.second);
            ctx->indent_deep -= 1;
            
            //",".
            if (++count < field.size()) {
                ctx->buffer.append(",");
            }
            json_encode_endline(ctx);
        }

        json_encode_indent(ctx, true);
        ctx->buffer.append("}");
    }
};

template<class Value> struct json_encoder<std::vector<Value>> {
    static void encode(json_encoding_ctx *ctx, const std::vector<Value> &field) {
        json_encode_indent(ctx, ctx->indent_head);
        ctx->buffer.append("[");
        json_encode_endline(ctx);
        
        size_t count = 0;
        for (auto &value : field) {
            //value.
            ctx->indent_head = true;
            ctx->indent_deep += 1;
            json_encoder<Value>::encode(ctx, value);
            ctx->indent_deep -= 1;
            
            //",".
            if (++count < field.size()) {
                ctx->buffer.append(",");
            }
            json_encode_endline(ctx);
        }
        
        json_encode_indent(ctx, true);
        ctx->buffer.append("]");
    }
};

template<class Object> struct json_encoder {
    static void encode(json_encoding_ctx *ctx, const Object &field) {
        field.encode_with_ctx(ctx);
    }
};

//"json_object" will use template "json_encoder<std::map<k, v>>" matching.
template<> struct json_encoder<encodable_field_base *> {
    static void encode(json_encoding_ctx *ctx, encodable_field_base *field) {
        field->on_encode(ctx);
    }
};

template<> struct json_encoder<std::string> {
    static void encode(json_encoding_ctx *ctx, const std::string &field) {
        json_encode_string(ctx, field);
    }
};

template<> struct json_encoder<double> {
    static void encode(json_encoding_ctx *ctx, double field) {
        json_encode_double(ctx, field);
    }
};

template<> struct json_encoder<bool> {
    static void encode(json_encoding_ctx *ctx, bool field) {
        json_encode_bool(ctx, field);
    }
};

//paser:

d_exportable bool json_parse_space (json_parsing_ctx *ctx, bool throwing);
d_exportable bool json_parse_token (json_parsing_ctx *ctx, bool throwing, char token);
d_exportable bool json_parse_value (json_parsing_ctx *ctx, bool throwing);
d_exportable bool json_parse_object(json_parsing_ctx *ctx, bool throwing);
d_exportable bool json_parse_array (json_parsing_ctx *ctx, bool throwing);
d_exportable bool json_parse_string(json_parsing_ctx *ctx, bool throwing, std::string *value);
d_exportable bool json_parse_double(json_parsing_ctx *ctx, bool throwing, double      *value);
d_exportable bool json_parse_bool  (json_parsing_ctx *ctx, bool throwing, bool        *value);

template<class Key, class Value> struct json_parser<std::map<Key, Value>> {
    static void parse(json_parsing_ctx *ctx, std::map<Key, Value> *field) {
        json_parse_token(ctx, true, '{');
        
        //if is empty object.
        if (json_parse_token(ctx, false, '}')) {
            return;
        }
        
        while (true) {
            //key.
            ctx->from_string = true;
            Key key;
            json_parser<Key>::parse(ctx, &key);
            
            //":".
            json_parse_token(ctx, true, ':');
            
            //value.
            ctx->from_string = false;
            Value value;
            json_parser<Value>::parse(ctx, &value);
            
            //"}" or ",".
            if (json_parse_token(ctx, false, '}')) {
                break;
            }
            json_parse_token(ctx, true, ',');
        }
    }
};

template<class Value> struct json_parser<std::vector<Value>> {
    static void parse(json_parsing_ctx *ctx, std::vector<Value> *field) {
        json_parse_token(ctx, true, '[');
        
        //if is empty array.
        if (json_parse_token(ctx, false, ']')) {
            return;
        }
        
        while (true) {
            //value.
            Value value;
            json_parser<Value>::parse(ctx, &value);
            
            //"]" or ",".
            if (json_parse_token(ctx, false, ']')) {
                break;
            }
            json_parse_token(ctx, true, ',');
        }
    }
};

template<class Object> struct json_parser {
    static void parse(json_parsing_ctx *ctx, Object *field) {
        field->parse_with_ctx(ctx);
    }
};

template<> struct json_parser<std::string> {
    static void parse(json_parsing_ctx *ctx, std::string *field) {
        json_parse_string(ctx, true, field);
    }
};

template<> struct json_parser<double> {
    static void parse(json_parsing_ctx *ctx, json<double> *field) {
        double value = 0;
        json_parse_double(ctx, true, &value);
        *field = value;
    }
    
    static void parse(json_parsing_ctx *ctx, double *field) {
        json_parse_double(ctx, true, field);
    }
};

template<> struct json_parser<bool> {
    static void parse(json_parsing_ctx *ctx, json<bool> *field) {
        bool value = false;
        json_parse_bool(ctx, true, &value);
        *field = value;
    }
    
    static void parse(json_parsing_ctx *ctx, bool *field) {
        json_parse_bool(ctx, true, field);
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

#define d_json_object(cls, ...) \
/**/    d_encodable_object(cls, dash::json_object, __VA_ARGS__)
