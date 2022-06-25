#include "dencodable.h"
#include "dlazy.h"

namespace dash {

static lazy<std::map<size_t, size_t>> s_ranges;

encodable::encodable(const std::string &key, size_t size) {
    if (key.empty()) {
        return;
    }

    auto bgn = (size_t)this;
    auto end = (size_t)this + size;
    s_ranges->insert({ bgn, end });

    _key = key;
}

encodable::~encodable() {
    s_ranges->erase((size_t)this);
}

const std::string encodable::key() const {
    return _key;
}

void encodable::encode(void *context) const {}
void encodable::decode(void *context) {}

void collect_sub_encodable(encodable_object *obj, size_t size) {
    auto bgn = (size_t)obj;
    auto end = (size_t)obj + size;

    auto fit = [=](const std::pair<size_t, size_t> &pair) {
        return bgn < pair.first && pair.second <= end;
    };

    for (auto it = s_ranges->begin(); it != s_ranges->end(); ) {
        if (fit(*it)) {
            //insert to the parent.
            auto sub = (encodable *)it->first;
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
