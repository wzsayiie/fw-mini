#include "djson.h"

namespace dash {

//object:

std::string json_object::encode() const {
    json_encoding_ctx ctx;
    encode_with_ctx(&ctx);
    return ctx.buffer;
}

bool json_object::parse(const std::string &text) {
    try {
        clear();
        
        json_parsing_ctx ctx;
        ctx.ptr = text.c_str();
        ctx.end = text.c_str() + text.size();
        parse_with_ctx(&ctx);
        
        return true;
        
    } catch (...) {
        clear();
        return false;
    }
}

void json_object::clear() {
    for (auto &pair : *this) {
        pair.second->on_clear();
    }
}

void json_object::encode_with_ctx(json_encoding_ctx *ctx) const {
    json_encoder<std::map<std::string, encodable_field_base *>>::encode(ctx, *this);
}

void json_object::parse_with_ctx(json_parsing_ctx *ctx) {
    json_parse_token(ctx, true, '{');
    
    //if is empty object.
    if (json_parse_token(ctx, false, '}')) {
        return;
    }
    
    while (true) {
        //key.
        std::string key;
        json_parse_string(ctx, true, &key);
        
        //":".
        json_parse_token(ctx, true, ':');
        
        //value.
        auto it = find(key);
        if (it == end()) {
            //ignore this value.
            json_parse_value(ctx, true);
        }
        it->second->on_parse(ctx);
        
        //"}" or ",".
        if (json_parse_token(ctx, false, '}')) {
            break;
        }
        json_parse_token(ctx, true, ',');
    }
}

//encoder:

void json_encode_indent(json_encoding_ctx *ctx, bool need) {
    if (!need) {
        return;
    }
    
    for (int deep = 0; deep < ctx->indent_deep; ++deep) {
        ctx->buffer.append("  ");
    }
}

void json_encode_endline(json_encoding_ctx *ctx) {
    ctx->buffer.append("\n");
}

void json_encode_space(json_encoding_ctx *ctx) {
    ctx->buffer.append(" ");
}

void json_encode_string(json_encoding_ctx *ctx, const std::string &value) {
    json_encode_indent(ctx, ctx->indent_head);
    ctx->buffer.append("\"");
    
    for (char chr : value) {
        switch (chr) {
            //NOTE: unicode escape "\u" is unsupported.
            
            case '\"': ctx->buffer.append("\\\""); break;
            case '\\': ctx->buffer.append("\\\\"); break;
            case '/' : ctx->buffer.append("\\/" ); break;
            case '\b': ctx->buffer.append("\\b" ); break;
            case '\f': ctx->buffer.append("\\f" ); break;
            case '\n': ctx->buffer.append("\\n" ); break;
            case '\r': ctx->buffer.append("\\r" ); break;
            case '\t': ctx->buffer.append("\\t" ); break;
                
            default: ctx->buffer.push_back(chr);
        }
    }
    
    ctx->buffer.append("\"");
}

void json_encode_double(json_encoding_ctx *ctx, double value) {
    json_encode_indent(ctx, ctx->indent_head);
    
    if (ctx->to_string) {
        ctx->buffer.append("\"");
        ctx->buffer.append(std::to_string(value));
        ctx->buffer.append("\"");
    } else {
        ctx->buffer.append(std::to_string(value));
    }
}

void json_encode_bool(json_encoding_ctx *ctx, bool value) {
    json_encode_indent(ctx, ctx->indent_head);
    
    if (ctx->to_string) {
        ctx->buffer.append(value ? "\"true\"" : "\"false\"");
    } else {
        ctx->buffer.append(value ? "true" : "false");
    }
}

//parser:

bool json_parse_space(json_parsing_ctx *ctx, bool throwing) {
    return false;
}

bool json_parse_token(json_parsing_ctx *ctx, bool throwing, char token) {
    return false;
}

bool json_parse_value(json_parsing_ctx *ctx, bool throwing) {
    return false;
}

bool json_parse_object(json_parsing_ctx *ctx, bool throwing) {
    return false;
}

bool json_parse_array(json_parsing_ctx *ctx, bool throwing) {
    return false;
}

bool json_parse_string(json_parsing_ctx *ctx, bool throwing, std::string *value) {
    return false;
}

bool json_parse_double(json_parsing_ctx *ctx, bool throwing, double *value) {
    return false;
}

bool json_parse_bool(json_parsing_ctx *ctx, bool throwing, bool *value) {
    return false;
}

} //end dash.
