#include "dencodable.h"
#include "dlazy.h"

namespace dash {

//field:

static lazy<std::map<size_t, size_t>> s_ranges;

encodable_field_base::encodable_field_base(const std::string &key) {
    if (key.empty()) {
        return;
    }

    auto bgn = (size_t)this;
    auto end = (size_t)this + sizeof(*this);
    s_ranges->insert({ bgn, end });

    _key = key;
}

encodable_field_base::~encodable_field_base() {
    s_ranges->erase((size_t)this);
}

const std::string &encodable_field_base::key() const {
    return _key;
}

//object:

void collect_sub_encodable(encodable_object *obj, size_t size) {
    auto bgn = (size_t)obj;
    auto end = (size_t)obj + size;

    auto fit = [=](const std::pair<size_t, size_t> &pair) {
        return bgn < pair.first && pair.second < end;
    };

    for (auto it = s_ranges->begin(); it != s_ranges->end(); ) {
        if (fit(*it)) {
            //insert to the parent.
            auto sub = (encodable_field_base *)it->first;
            obj->insert({ sub->key(), sub });

            //remove from the pool.
            it = s_ranges->erase(it);

        } else {
            //next.
            ++it;
        }
    }
}

} //end dash.
