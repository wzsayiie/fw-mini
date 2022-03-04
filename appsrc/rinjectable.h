#pragma once

#include "rfunction.h"

namespace reflect {

//ftable:
//

class ftable : public dash::extends<ftable, dash::object> {
public:
    void add(const char *name, const base_function::ptr &func);
    base_function::ptr find(const char *name);

private:
    std::map<symbol *, base_function::ptr> _functions;
};

//injectable:
//

template<> struct type_name_of<class injectable> {
    static constexpr const char *const name = "injectable";
};

class injectable : public extends<injectable, object> {
public:
    void inject(const ftable::ptr &ftab);
    base_function::ptr find_injected(const char *name);

private:
    ftable::ptr _ftab;
};

} //end reflect.
