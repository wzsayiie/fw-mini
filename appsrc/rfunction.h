#pragma once

#include "robject.h"

namespace reflect {

template<> struct class_name_of<class function> {
    static symbol *value() {
        static symbol *sym = symbol::make("function");
        return sym;
    }
};

class function : public extends<function, object> {
public:
    static any  get_this_arg() { return get_argument(0); }
    static any  get_argument(int index);
    static void return_value(const any &value);

    any call_with_args(const std::vector<any> &args) const;

protected:
    virtual void on_call() const = 0;
};

template<class> class func;
template<class Ret, class... Args> class func<Ret (Args...)> : public function {
public:
    typedef std::shared_ptr<func> ptr;

    template<class Fn> func(const Fn &fn) {
        _fn = fn;
    }

    any call_with_this(const any &this_arg, Args... args) const {
        std::vector<any> list = { this_arg, args... };
        return call_with_args(list);
    }

    any call(Args... args) const {
        std::vector<any> list = { nullptr, args... };
        return call_with_args(list);
    }

protected:
    void on_call() const override {
        Ret returned = caller<0>::call(_fn);
        return_value(returned);
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

    template<int Index> struct caller {
        template<class... Unfold> static Ret call(const std::function<Ret (Args...)> &fn, Unfold... unfold) {
            auto value = (typename type_at<Index, void (Args...)>::type)get_argument(Index + 1);
            return caller<Index + 1>::call(fn, unfold..., value);
        }
    };
    template<> struct caller<arg_count<void (Args...)>::value> {
        template<class... Unfold> static Ret call(const std::function<Ret (Args...)> &fn, Unfold... unfold) {
            return fn(unfold...);
        }
    };

    std::function<Ret (Args...)> _fn;
};

} //end reflect.
