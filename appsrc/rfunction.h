#pragma once

#include "robject.h"

namespace reflect {

any  get_argument(int index);
void return_value(const any &value);

inline any get_this_arg() {
    return get_argument(0);
}

reflect_reflectable_class(base_function);
class base_function : public extends<base_function, object> {
public:
    any call_with_args(const std::vector<any> &args) const;

protected:
    virtual void on_call() const = 0;
};

template<class> class function;
template<class Ret, class... Args> class function<Ret (Args...)>
    : public extends<function<Ret (Args...)>, base_function>
{
public:
    template<class Fcn> function(const Fcn &fcn) {
        _fcn = fcn;
    }

    any call_with_this(const any &this_arg, Args... args) const {
        std::vector<any> list = { this_arg, args... };
        return this->call_with_args(list);
    }

    any call(Args... args) const {
        std::vector<any> list = { nullptr, args... };
        return this->call_with_args(list);
    }

protected:
    void on_call() const override {
        caller<0>::call(_fcn);
    }

private:
    template<class> struct arg_count;
    template<class FirstArg, class... OtherArgs> struct arg_count<void (FirstArg, OtherArgs...)> {
        static const int value = arg_count<void (OtherArgs...)>::value + 1;
    };
    template<> struct arg_count<void ()> {
        static const int value = 0;
    };

    template<int, class> struct type_at;
    template<int Index, class FirstArg, class... OtherArgs> struct type_at<Index, void (FirstArg, OtherArgs...)> {
        typedef typename type_at<Index - 1, void (OtherArgs...)>::type type;
    };
    template<class FirstArg, class... OtherArgs> struct type_at<0, void (FirstArg, OtherArgs...)> {
        typedef FirstArg type;
    };
    
    template<class Result> struct returner {
        template<class... Unfold> static void retv(const std::function<Ret (Args...)> &fcn, Unfold... unfold) {
            Result result = fcn(unfold...);
            return_value(result);
        }
    };
    template<> struct returner<void> {
        template<class... Unfold> static void retv(const std::function<Ret (Args...)> &fcn, Unfold... unfold) {
            fcn(unfold...);
        }
    };

    template<int Index> struct caller {
        template<class... Unfold> static void call(const std::function<Ret (Args...)> &fcn, Unfold... unfold) {
            auto value = (typename type_at<Index, void (Args...)>::type)get_argument(Index + 1);
            caller<Index + 1>::call(fcn, unfold..., value);
        }
    };
    template<> struct caller<arg_count<void (Args...)>::value> {
        template<class... Unfold> static void call(const std::function<Ret (Args...)> &fcn, Unfold... unfold) {
            returner<Ret>::retv(fcn, unfold...);
        }
    };

    std::function<Ret (Args...)> _fcn;
};

} //end reflect.
