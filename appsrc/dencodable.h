#pragma once

#include "dobject.h"
#include "dstd.h"

namespace dash {

class dash_exportable encodable {
public:
    encodable(const std::string &key, size_t size);
    virtual ~encodable();

public:
    const std::string key() const;

protected:
    virtual void encode(void *context) const;
    virtual void decode(void *context);

private:
    std::string _key;
};

struct dash_exportable encodable_object
    : virtual_object
    , std::map<std::string, encodable *>
{
};

dash_exportable
void collect_sub_encodable(encodable_object *obj, size_t size);

} //end dash.

#define dash_encodable_object(cls)                          \
/**/    cls() {                                             \
/**/        dash::collect_sub_encodable(this, sizeof(cls)); \
/**/    }
