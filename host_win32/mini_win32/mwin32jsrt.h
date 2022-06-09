#pragma once

#include "mjsvm.h"

#define USE_EDGEMODE_JSRT
#include <jsrt.h>

m_class(MWin32JsVM, MVirtualJsVM) {
public:
    static void install();

public:
    MWin32JsVM();
    ~MWin32JsVM();

public:
    void setExceptionListener(const MBaseFunction::ptr &listener) override;
    void registerFunction(const std::string &name, const MBaseFunction::ptr &func) override;
    void evaluate(const std::string &name, const std::string &script) override;

private:
    MBaseFunction::ptr mExceptionListener;

    JsRuntimeHandle mRuntime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef    mContext = JS_INVALID_REFERENCE;
    JsSourceContext mCodeId  = 0;
};
