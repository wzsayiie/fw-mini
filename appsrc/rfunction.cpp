#include "rfunction.h"

namespace reflect {

struct stack_frame {
    std::vector<any> args;
    any returned;
};
static dash::lazy<std::vector<stack_frame>> s_frames;

any get_argument(int index) {
    if (s_frames->empty()) {
        return any();
    }

    std::vector<any> &args = s_frames->rbegin()->args;
    if (index < 0 || (int)args.size() <= index) {
        return any();
    }

    return args.at(index);
}

void return_value(const any &value) {
    if (!s_frames->empty()) {
        s_frames->rbegin()->returned = value;
    }
}

any base_function::call_with_args(const std::vector<any> &args) const {
    stack_frame frame = { args };
    s_frames->push_back(frame);

    on_call();

    any returned = s_frames->rbegin()->returned;
    s_frames->pop_back();
    return returned;
}

} //end reflect.
