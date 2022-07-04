#pragma once

#include "mjsvm.h"

#define USE_EDGEMODE_JSRT
#include <jsrt.h>

m_class(MWin32JsVM, MJsVM)
{
public:
    static void install();

public:
    MWin32JsVM();
    ~MWin32JsVM();

public:
    JsValueRef   getJsValue (const reflect::any &cppValue);
    reflect::any getCppValue(JsValueRef jsValue);

protected:
    void onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func) override;
    void onEvaluate(const std::string &name, const std::string &script) override;

private:
    static JsValueRef onCallNativeFunction(
        JsValueRef callee, bool isConstruct, JsValueRef *args, USHORT argc, void *custom);

    static void onCollectJsObject(JsRef value, void *custom);

private:
    JsRuntimeHandle mRuntime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef    mContext = JS_INVALID_REFERENCE;
    JsSourceContext mCodeId  = 0;

    std::map<intptr_t, MBaseFunction::ptr> mNativeFunctions;
    std::map<JsValueRef, reflect::object::ptr> mObjectMap;
};
