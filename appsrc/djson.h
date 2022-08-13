#pragma once

#include "dencodable.h"

namespace dash {

const char *const json_indent_text  = "  ";
const char *const json_endline_text = "\n";

struct json_encode_context {
    std::string buffer;
    int indent = 0;
};

struct json_parse_context {
};

template<class Type> class json : public encodable<Type> {
public:
    typedef encodable<Type> _inheritable;

    using _inheritable::_inheritable;
    using _inheritable::operator=;

public:
    void on_encode(void *context) const override {
    }

    void on_parse(void *context) override {
    }
};

class d_exportable json_object : public encodable_object {
public:
    std::string encode() const;
    void parse(const std::string &text);

public:
    void on_encode(void *context) const override;
    void on_parse (void *context)       override;
};

} //end dash.

#define d_json_object(cls) d_encodable_object(cls)
