#include "dencodable.h"
#include "dlazy.h"

namespace dash {

//field:

static lazy<std::map<size_t, size_t>> s_ranges;

encodable_field::encodable_field(const std::string &key) {
    if (key.empty()) {
        return;
    }

    auto bgn = (size_t)this;
    auto end = (size_t)this + sizeof(*this);
    s_ranges->insert({ bgn, end });

    _key = key;
}

encodable_field::~encodable_field() {
    s_ranges->erase((size_t)this);
}

void encodable_field::on_encode(void *context) const {}
void encodable_field::on_parse (void *context)       {}
void encodable_field::on_clear ()                    {}

const std::string &encodable_field::key() const {
    return _key;
}

//object:

encodable_field_ptrdiff::encodable_field_ptrdiff() {
    _value = 0;
}

encodable_field_ptrdiff::encodable_field_ptrdiff(size_t value) {
    _value = value;
}

size_t encodable_field_ptrdiff::value() const {
    return _value;
}

void encodable_object::collect(encodable_object *obj, size_t size) {
    auto bgn = (size_t)obj;
    auto end = (size_t)obj + size;

    auto fit = [=](const std::pair<size_t, size_t> &pair) {
        return bgn < pair.first && pair.second < end;
    };

    for (auto it = s_ranges->begin(); it != s_ranges->end(); ) {
        if (fit(*it)) {
            //insert to the parent.
            auto field = (encodable_field *)it->first;
            auto diff  = it->first - bgn;
            obj->insert({ field->key(), diff });

            //remove from the pool.
            it = s_ranges->erase(it);

        } else {
            //next.
            ++it;
        }
    }
}

encodable_field *encodable_object::field_at(const encodable_field_ptrdiff &diff) const {
    return (encodable_field *)((uint8_t *)this + diff.value());
}

} //end dash.
