#pragma once

#include <type_traits>
#include "minikit.h"

class CComparableLambda {

public:
    typedef void *Id[3];

    bool operator< (const CComparableLambda &that) const;
    bool operator==(const CComparableLambda &that) const;
    bool operator!=(const CComparableLambda &that) const;

    bool operator==(nullptr_t) const;
    bool operator!=(nullptr_t) const;

    operator bool() const;

protected:
    void setId(void *a, void *b, void *c);
    void setRandomId();

private:
    int cmp(const Id &that) const;

    Id _id = { nullptr };
};

template<typename T> class CLambda;

template<typename R, typename... A> class CLambda<R (A...)> : public CComparableLambda {

public:
    template<typename C> /**/ CLambda  (C *obj, R (C::*fn)(A...))            { assign(obj, fn); }
    template<typename L> /**/ CLambda  (const L &func)                       { assign(func   ); }
    /* std::function  */ /**/ CLambda  (const std::function<R (A...)> &func) { assign(func   ); }
    /* global func    */ /**/ CLambda  (R (*func)(A...))                     { assign(func   ); }
    /* null value     */ /**/ CLambda  (nullptr_t)                           { assign(nullptr); }
    /* null value     */ /**/ CLambda  ()                                    { assign(nullptr); }
    template<typename L> void operator=(const L &func)                       { assign(func   ); }
    /* std::function  */ void operator=(const std::function<R (A...)> &func) { assign(func   ); }
    /* global func    */ void operator=(R (*func)(A...))                     { assign(func   ); }
    /* null value     */ void operator=(nullptr_t)                           { assign(nullptr); }

    template<typename C> void assign(C *object, R (C::*func)(A...)) {
        if (!object || !func) {
            reset();
            return;
        }

        void *a = object;
        void *b = ((void **)&func)[0];

        //NOTE: on the implementations of clang and gcc,
        //a member function pointer occupies two pointers space.
      #if M_CPL_CLANG || M_CPL_GCC
        void *c = ((void **)&func)[1];
      #else
        void *c = nullptr;
      #endif

        setId(a, b, c);

        _func = [=](A... a) -> R {
            return (object->*func)(a...);
        };
    }

    template<typename L> void assign(const L &func) {
        setRandomId();
        _func = func;
    }
    
    void assign(const std::function<R (A...)> &func) {
        if (func) {
            setRandomId();
            _func = func;
        } else {
            reset();
        }
    }

    void assgin(R (*func)(A...)) {
        if (func) {
            setId(func, nullptr, nullptr);
            _func = func;
        } else {
            reset();
        }
    }
    
    void assign(nullptr_t) {
        reset();
    }

    void reset() {
        setId(nullptr, nullptr, nullptr);
        _func = nullptr;
    }

    R operator()(A... args) const {
        if (_func) {
            return _func(args...);
        } else {
            return R();
        }
    }

private:
    std::function<R (A...)> _func;
};

#define M(func) _CLambdaMake(this, &std::remove_reference<decltype(*this)>::type::func)

template<typename T> void _CLambdaMake();

template<typename C, typename R, typename... A>
    CLambda<R (A...)> _CLambdaMake(C *object, R (C::*func)(A...)) {
        return CLambda<R (A...)>(object, func);
    }
