#pragma once

#include "mjsvm.h"

#define USE_EDGEMODE_JSRT
#include <jsrt.h>

//object pool:

m_class(MWin32JsObjectPool, MObject)
{
public:
    static MWin32JsObjectPool *instance();

public:
    JsValueRef   getJsValue (const reflect::any &cppValue);
    reflect::any getCppValue(JsValueRef jsValue);

    void whenCollectCpp(reflect::object *obj);

private:
    static void CALLBACK whenCollectJs(JsRef value, void *custom);

private:
    //js hold cpp objects.
    std::map<JsValueRef          , reflect::object::ptr> mCppObjects  ;
    std::map<reflect::object::ptr, JsValueRef          > mCppJsHolders;
    //cpp hold js objects.
    std::map<reflect::object *   , JsValueRef          > mJsObjects   ;
    std::map<JsValueRef          , reflect::object *   > mJsCppHolders;
};

//js function:

m_class(MWin32JsFunction, MGenericFunction)
{
public:
    MWin32JsFunction(JsValueRef jsFunc);
    ~MWin32JsFunction();

protected:
    void on_call() const override;

private:
    JsValueRef mJsFunc;
};

//virtual machine:

m_class(MWin32JsVM, MJsVM)
{
public:
    static void install();

public:
    MWin32JsVM();
    ~MWin32JsVM();

protected:
    void onRegisterFunction(const std::string &name, const MGenericFunction::ptr &func) override;
    void onEvaluate(const std::string &name, const std::string &script) override;

private:
    static JsValueRef CALLBACK onCallNativeFunction(
        JsValueRef callee, bool isConstruct, JsValueRef *args, USHORT argc, void *custom);

private:
    void AppendExceptionInfo(
        std::wstring *appended, const wchar_t *desc, JsValueRef obj, const wchar_t *keyName);

private:
    std::vector<MGenericFunction::ptr> mNativeFunctions;

    JsRuntimeHandle mRuntime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef    mContext = JS_INVALID_REFERENCE;
    JsSourceContext mCodeId  = 0;
};
