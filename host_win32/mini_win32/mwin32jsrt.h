#pragma once

#include "mjsvm.h"

#define USE_EDGEMODE_JSRT
#include <jsrt.h>

m_class(MWin32JsVM, MJsVM) {
public:
    static void install();

public:
    MWin32JsVM();
    ~MWin32JsVM();

public:
    void onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func) override;
    void onEvaluate(const std::string &name, const std::string &script) override;

private:
    JsRuntimeHandle mRuntime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef    mContext = JS_INVALID_REFERENCE;
    JsSourceContext mCodeId  = 0;
};
