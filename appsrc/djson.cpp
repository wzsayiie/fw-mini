#include "djson.h"
#include <cstring>
#include "dprint.h"

namespace dash {

//object:

std::string json_object::encode() const {
    json_encoding_ctx ctx;
    json_encoder<json_object>::encode(this, &ctx);
    return ctx.buffer;
}

bool json_object::parse(const std::string &text) {
    try {
        clear();
        
        json_parsing_ctx ctx;
        ctx.ptr = text.c_str();
        ctx.end = text.c_str() + text.size();
        json_parser<json_object>::parse(this, &ctx);
        
        return true;
        
    } catch (...) {
        clear();
        return false;
    }
}

void json_object::clear() {
    for (auto &[_, diff] : *this) {
        field_at(diff)->on_clear();
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
            default  : ctx->buffer.push_back(chr);
        }
    }
    
    ctx->buffer.append("\"");
}

void json_encode_double(json_encoding_ctx *ctx, double value) {
    json_encode_indent(ctx, ctx->indent_head);
    
    std::string string = std::to_string(value);
    
    //remove "0" at the end.
    const char *ptr = string.c_str();
    const char *end = string.c_str() + string.size();
    while (ptr + 1 < end && end[-1] == '0') {
        end -= 1;
    }
    if (ptr + 1 < end && end[-1] == '.') {
        end -= 1;
    }
    
    if (ctx->this_is_key) {
        ctx->buffer.append("\"");
        ctx->buffer.append(ptr, end);
        ctx->buffer.append("\"");
    } else {
        ctx->buffer.append(ptr, end);
    }
}

void json_encode_bool(json_encoding_ctx *ctx, bool value) {
    json_encode_indent(ctx, ctx->indent_head);
    
    if (ctx->this_is_key) {
        ctx->buffer.append(value ? "\"true\"" : "\"false\"");
    } else {
        ctx->buffer.append(value ? "true" : "false");
    }
}

//parser:

void json_set_intent(json_parsing_ctx *ctx, const char *intent) {
    ctx->handle = ctx->ptr;
    ctx->intent = intent;
}

void json_skip_blank(json_parsing_ctx *ctx) {
    while (ctx->ptr < ctx->end) {
        switch (*ctx->ptr) {
            case ' ' : ctx->ptr += 1; break;
            case '\t': ctx->ptr += 1; break;
            case '\n': ctx->ptr += 1; break;
            case '\r': ctx->ptr += 1; break;
            default  : return;
        }
    }
}

char json_will_read(json_parsing_ctx *ctx) {
    json_skip_blank(ctx);
    
    if (ctx->ptr < ctx->end) {
        return *ctx->ptr;
    } else {
        return 0;
    }
}

void json_exception(json_parsing_ctx *ctx) noexcept(false) {
    const char *end = std::min   (ctx->handle + 40, ctx->end);
    std::string pos = std::string(ctx->handle     , end     );
    
    print("json parse error, want", ctx->intent, "at:", pos);
    
    throw 0;
}

void json_parse_token(json_parsing_ctx *ctx, const char *target) noexcept(false) {
    json_skip_blank(ctx);
    
    size_t size = strlen(target);
    if (ctx->ptr + size <= ctx->end && strncmp(ctx->ptr, target, size) == 0) {
        ctx->ptr += size;
    } else {
        json_exception(ctx);
    }
}

void json_parse_ignore(json_parsing_ctx *ctx) noexcept(false) {
    char ch = json_will_read(ctx);
    
    if (ch == 'n' /* null */ || ch == '{') {
        json_parser<std::map<std::string, json_ignore>>::parse(nullptr, ctx);
        
    } else if (ch == 'n' /* null */ || ch == '[') {
        json_parser<std::vector<json_ignore>>::parse(nullptr, ctx);
        
    } else if (ch == '\"') {
        json_parse_string(ctx, nullptr);
        
    } else if (ch == '-' || ('0' <= ch && ch <= '9')) {
        json_parse_double(ctx, nullptr);
        
    } else if (ch == 'f' || ch == 't') {
        json_parse_bool(ctx, nullptr);
        
    } else {
        json_set_intent(ctx, "value");
        json_exception (ctx);
    }
}

void json_parse_string(json_parsing_ctx *ctx, std::string *outer) noexcept(false) {
    json_set_intent (ctx, "string");
    json_parse_token(ctx, "\"");
    
    std::string string;
    while (ctx->ptr < ctx->end) {
        if (*ctx->ptr == '\\' && ctx->ptr + 1 < ctx->end) {
            switch (ctx->ptr[1]) {
                //NOTE: unicode escape "\u" is unsupported.
                
                case '"' : string.push_back('\"'); break;
                case '\\': string.push_back('\\'); break;
                case '/' : string.push_back('/' ); break;
                case 'b' : string.push_back('\b'); break;
                case 'f' : string.push_back('\f'); break;
                case 'n' : string.push_back('\n'); break;
                case 'r' : string.push_back('\r'); break;
                case 't' : string.push_back('\t'); break;
                default  : ;
            }
            ctx->ptr += 2;
            
        } else if (*ctx->ptr == '\\') {
            ctx->ptr += 1;
            
        } else if (*ctx->ptr == '\"') {
            //end of the string.
            if (outer) {
                *outer = string;
            }
            ctx->ptr += 1;
            return;
            
        } else {
            string.push_back(*ctx->ptr);
            ctx->ptr += 1;
        }
    }
    
    json_exception(ctx);
}

void json_parse_double(json_parsing_ctx *ctx, double *outer) noexcept(false) {
    json_set_intent(ctx, "number");
    
    if (ctx->this_is_key) {
        json_parse_token(ctx, "\"");
    }
    
    try {
        size_t size = 0;
        double item = std::stod(ctx->ptr, &size);

        if (outer) {
            *outer = item;
        }
        ctx->ptr += size;
        
    } catch (...) {
        json_exception(ctx);
    }
    
    if (ctx->this_is_key) {
        json_parse_token(ctx, "\"");
    }
}

void json_parse_bool(json_parsing_ctx *ctx, bool *outer) noexcept(false) {
    json_set_intent(ctx, "boolean");
    
    if (ctx->this_is_key) {
        json_parse_token(ctx, "\"");
    }
    
    char ch = json_will_read(ctx);
    switch (ch) {
        case 'f': {
            json_parse_token(ctx, "false");
            if (outer) {
                *outer = false;
            }
            break;
        }
        case 't': {
            json_parse_token(ctx, "true");
            if (outer) {
                *outer = true;
            }
            break;
        }
        default : {
            json_exception(ctx);
        }
    }
    
    if (ctx->this_is_key) {
        json_parse_token(ctx, "\"");
    }
}

} //end dash.
