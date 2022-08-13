#include "djson.h"

namespace dash {

std::string json_object::encode() const {
    json_encode_context context;

    try {
        on_encode(&context);
        return context.buffer;

    } catch (...) {
        return "";
    }
}

void json_object::parse(const std::string &text) {
}

void json_object::on_encode(void *context) const  {
    auto ctx = (json_encode_context *)context;

    ctx->buffer.append("{");
    ctx->buffer.append(json_endline_text);

    for (auto pair = begin(); pair != end(); ++pair) {
        //delimiter.
        if (pair != begin()) {
            ctx->buffer.append(",");
            ctx->buffer.append(json_endline_text);
        }
        //subitem.
        ctx->indent += 1;
        pair->second->on_encode(ctx);
        ctx->indent -= 1;
    }

    ctx->buffer.append("}");
}

void json_object::on_parse(void *context) {
}

} //end dash.
