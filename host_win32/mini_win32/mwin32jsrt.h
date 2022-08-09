#pragma once

#include "mjsvm.h"

#define USE_EDGEMODE_JSRT
#include <jsrt.h>

//object pool:

m_class(MWin32JsObjectPool, MObject)
{
public:
    static MWin32JsObjectPool *instance();
    static void clearInstance();

public:
    ~MWin32JsObjectPool();

public:
    JsValueRef   getJsValue (const reflect::any &cppValue);
    reflect::any getCppValue(JsValueRef jsValue);

    void collectCppObject(reflect::object *obj);

private:
    static MWin32JsObjectPool::ptr sInstance;

private:
    static void CALLBACK onCollectJsObject(JsRef value, void *custom);

private:
    std::map<JsValueRef          , reflect::object::ptr> mCppObjects;
    std::map<reflect::object::ptr, JsValueRef          > mCppHolders;
    std::map<reflect::object *   , JsValueRef          > mJsObjects ;
    std::map<JsValueRef          , reflect::object *   > mJsHolders ;
};

//js function:

m_class(MWin32JsFunction, MBaseFunction)
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
    void onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func) override;
    void onEvaluate(const std::string &name, const std::string &script) override;

private:
    static JsValueRef CALLBACK onCallNativeFunction(
        JsValueRef callee, bool isConstruct, JsValueRef *args, USHORT argc, void *custom);

private:
    void AppendExceptionInfo(
        std::wstring *appended, const wchar_t *desc, JsValueRef obj, const wchar_t *keyName);

private:
    std::vector<MBaseFunction::ptr> mNativeFunctions;

    JsRuntimeHandle mRuntime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef    mContext = JS_INVALID_REFERENCE;
    JsSourceContext mCodeId  = 0;
};
