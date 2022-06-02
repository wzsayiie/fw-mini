#pragma once

#include "rany.h"

namespace reflect {

//arguments and return value of current stack frame.
dash_exportable any  get_argument(int index);
dash_exportable void return_value(const any &value);

//function arguments count:

template<class> struct arg_count;

template<class First, class... Others> struct arg_count<void (First, Others...)> {
    static const int value = arg_count<void (Others...)>::value + 1;
};

template<> struct arg_count<void ()> {
    static const int value = 0;
};

//argument type:

template<int, class> struct type_at;

template<int Index, class First, class... Others> struct type_at<Index, void (First, Others...)> {
    typedef typename type_at<Index - 1, void (Others...)>::type type;
};

template<class First, class... Others> struct type_at<0, void (First, Others...)> {
    typedef First type;
};

//call and return value:

template<class> struct returner;

template<class Ret, class... Args> struct returner<Ret (Args...)> {
    static void ret(const std::function<Ret (Args...)> &fcn, Args... args) {
        Ret result = fcn(args...);
        return_value(result);
    }
};

template<class... Args> struct returner<void (Args...)> {
    static void ret(const std::function<void (Args...)> &fcn, Args... args) {
        fcn(args...);
    }
};

//unfold arguments and call:

template<int, int, class> struct caller;

template<int Index, int End, class Ret, class... Args> struct caller<Index, End, Ret (Args...)> {
    template<class... Unfold> static void call(const std::function<Ret (Args...)> &fcn, Unfold... unfold) {
        auto arg = get_argument(Index);
        auto val = query<typename type_at<Index, void (Args...)>::type>::from(arg);
        caller<Index + 1, End, Ret (Args...)>::call(fcn, unfold..., val);
    }
};

template<int Index, class Ret, class... Args> struct caller<Index, Index, Ret (Args...)> {
    template<class... Unfold> static void call(const std::function<Ret (Args...)> &fcn, Unfold... unfold) {
        returner<Ret (Args...)>::ret(fcn, unfold...);
    }
};

//base function:

class base_function;

template<> struct type_ids<base_function> {
    static constexpr const void *ids[] = { "base_function", nullptr };
};

class dash_exportable base_function : public extends<base_function, object> {
public:
    any call_with_args(const std::vector<any> &args) const;

protected:
    virtual void on_call() const = 0;
};

//function:

template<class> class function;

template<class Ret, class... Args> struct type_ids<function<Ret (Args...)>> {
    static constexpr const void *ids[] = {
        "function<",
        type_ids_esc, type_ids<Ret>::ids,
        "(",
        type_ids_esc, arg_types_ids<void (Args...)>::ids,
        ")>",
        nullptr
    };
};

template<class Ret> struct type_ids<function<Ret ()>> {
    static constexpr const void *ids[] = {
        "function<", type_ids_esc, type_ids<Ret>::ids, "()>", nullptr
    };
};

template<class Ret, class... Args> class function<Ret (Args...)>
    : public extends<function<Ret (Args...)>, base_function>
{
public:
    function() {
    }
    
    template<class Fcn> function(const Fcn &fcn) {
        _fcn = fcn;
    }

    any call(Args... args) const {
        std::vector<any> list = { args... };
        return this->call_with_args(list);
    }

protected:
    void on_call() const override {
        if (_fcn) {
            caller<0, arg_count<void (Args...)>::value, Ret (Args...)>::call(_fcn);
        }
    }

private:
    std::function<Ret (Args...)> _fcn;
};

} //end reflect.
