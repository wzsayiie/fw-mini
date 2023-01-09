#include "mwin32jsrt.h"
#include "mencode.h"

//object pool:

MWin32JsObjectPool *MWin32JsObjectPool::instance()
{
    //IMPORTANT: new a pointer object.
    //at the end of the program, static smart pointers will automatically destruct,
    //and other objects may call this function when they are destructing.
    static auto obj = new MWin32JsObjectPool::ptr(
        MWin32JsObjectPool::create()
    );
    return obj->get();
}

JsValueRef MWin32JsObjectPool::getJsValue(const reflect::any &cppValue)
{
    switch (cppValue.preferred_type())
    {
        case reflect::data_type::is_object:
        {
            reflect::object::ptr cppObject = cppValue.as_object_shared();

            //null.
            if (!cppObject)
            {
                JsValueRef nulled = JS_INVALID_REFERENCE;
                JsGetNullValue(&nulled);
                return nulled;
            }

            //query from cache:
            auto holder = mCppJsHolders.find(cppObject);
            if (holder != mCppJsHolders.end())
            {
                return holder->second;
            }

            auto held = mJsObjects.find(cppObject.get());
            if (held != mJsObjects.end())
            {
                return held->second;
            }

            //new object.
            JsValueRef jsObject = JS_INVALID_REFERENCE;
            JsCreateObject(&jsObject);

            JsSetObjectBeforeCollectCallback(jsObject, this, whenCollectJs);
            mCppObjects  [jsObject ] = cppObject;
            mCppJsHolders[cppObject] = jsObject;

            return jsObject;
        }

        case reflect::data_type::is_string:
        {
            std::u16string u16s = MU16StringFromU8(cppValue.as_chars());
            JsValueRef jsValue = JS_INVALID_REFERENCE;
            JsPointerToString((const wchar_t *)u16s.c_str(), u16s.size(), &jsValue);
            return jsValue;
        }

        case reflect::data_type::is_byte  :
        case reflect::data_type::is_int   :
        case reflect::data_type::is_int64 :
        case reflect::data_type::is_float :
        case reflect::data_type::is_double:
        {
            JsValueRef jsValue = JS_INVALID_REFERENCE;
            JsDoubleToNumber(cppValue.as_double(), &jsValue);
            return jsValue;
        }

        case reflect::data_type::is_bool:
        {
            JsValueRef jsValue = JS_INVALID_REFERENCE;
            JsBoolToBoolean(cppValue.as_bool(), &jsValue);
            return jsValue;
        }

        default:
        {
            JsValueRef undefined = JS_INVALID_REFERENCE;
            JsGetUndefinedValue(&undefined);
            return undefined;
        }
    }
}

reflect::any MWin32JsObjectPool::getCppValue(JsValueRef jsValue)
{
    JsValueType jsType = JsUndefined;
    JsGetValueType(jsValue, &jsType);

    switch (jsType)
    {
        case JsFunction:
        {
            //query from cache:
            auto holder = mJsCppHolders.find(jsValue);
            if (holder != mJsCppHolders.end())
            {
                return holder->second;
            }

            auto held = mCppObjects.find(jsValue);
            if (held != mCppObjects.end())
            {
                return held->second;
            }

            //new object:
            auto cppObject = MWin32JsFunction::create(jsValue);
            mJsObjects   [cppObject.get()] = jsValue;
            mJsCppHolders[jsValue        ] = cppObject.get();

            //NOTE: add reference count.
            JsAddRef(jsValue, nullptr);

            return cppObject;
        }

        case JsObject:
        {
            auto holder = mJsCppHolders.find(jsValue);
            if (holder != mJsCppHolders.end())
            {
                return holder->second;
            }

            auto held = mCppObjects.find(jsValue);
            if (held != mCppObjects.end())
            {
                return held->second;
            }

            return nullptr;
        }

        case JsString:
        {
            const wchar_t *cppValue = nullptr;
            size_t cppValueSize = 0;
            JsStringToPointer(jsValue, &cppValue, &cppValueSize);
            return MU8StringFromU16((const char16_t *)cppValue);
        }
        
        case JsNumber:
        {
            double cppValue = 0;
            JsNumberToDouble(jsValue, &cppValue);
            return cppValue;
        }

        case JsBoolean:
        {
            bool cppValue = false;
            JsBooleanToBool(jsValue, &cppValue);
            return cppValue;
        }

        default:
        {
            return nullptr;
        }
    }
}

void MWin32JsObjectPool::whenCollectCpp(reflect::object *obj)
{
    auto held = mJsObjects.find(obj);
    if (held != mJsObjects.end())
    {
        //NOTE: release reference count.
        JsRelease(held->second, nullptr);

        mJsCppHolders.erase(held->second);
        mJsObjects   .erase(obj);
    }
}

void MWin32JsObjectPool::whenCollectJs(JsRef value, void *custom)
{
    auto pool = (MWin32JsObjectPool *)custom;
    auto held = pool->mCppObjects.find(value);
    if (held != pool->mCppObjects.end())
    {
        pool->mCppJsHolders.erase(held->second);
        pool->mCppObjects  .erase(value);
    }
}

//js function:

MWin32JsFunction::MWin32JsFunction(JsValueRef jsFunc)
{
    mJsFunc = jsFunc;
}

MWin32JsFunction::~MWin32JsFunction()
{
    MWin32JsObjectPool::instance()->whenCollectCpp(this);
}

void MWin32JsFunction::on_call() const
{
    //build argument:
    std::vector<JsValueRef> jsArgs;

    //NOTE: first argument is "this".
    JsValueRef thisArg = JS_INVALID_REFERENCE;
    JsGetUndefinedValue(&thisArg);
    jsArgs.push_back(thisArg);

    auto count = reflect::get_arg_count();
    for (int i = 0; i < count; ++i)
    {
        reflect::any cppValue = reflect::get_arg_value(i);
        JsValueRef jsValue = MWin32JsObjectPool::instance()->getJsValue(cppValue);

        jsArgs.push_back(jsValue);
    }

    //call.
    JsValueRef jsRet = JS_INVALID_REFERENCE;
    JsCallFunction(mJsFunc, jsArgs.data(), (USHORT)jsArgs.size(), &jsRet);

    //return value.
    reflect::any cppRet = MWin32JsObjectPool::instance()->getCppValue(jsRet);
    reflect::return_value(cppRet);
}

//virtual machine:

void MWin32JsVM::install()
{
    auto obj = MWin32JsVM::create();
    setInstance(obj);
}

MWin32JsVM::MWin32JsVM()
{
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &mRuntime);
    JsCreateContext(mRuntime, &mContext);
    JsSetCurrentContext(mContext);
}

MWin32JsVM::~MWin32JsVM()
{
    JsRelease(mContext, nullptr);
    JsDisposeRuntime(mRuntime);
}

void MWin32JsVM::onRegisterFunction(const std::string &name, const MGenericFunction::ptr &func)
{
    auto funcId = (intptr_t)mNativeFunctions.size();
    mNativeFunctions.push_back(func);

    //function key.
    std::u16string u16name = MU16StringFromU8(name.c_str());
    JsValueRef     funcKey = JS_INVALID_REFERENCE;
    JsGetPropertyIdFromName((const wchar_t *)u16name.c_str(), &funcKey);

    //function value.
    JsValueRef funcValue = JS_INVALID_REFERENCE;
    JsCreateFunction(onCallNativeFunction, (void *)funcId, &funcValue);

    //add to the global object.
    JsValueRef globalObject = JS_INVALID_REFERENCE;
    JsGetGlobalObject(&globalObject);
    JsSetProperty(globalObject, funcKey, funcValue, true);
}

void MWin32JsVM::onEvaluate(const std::string &name, const std::string &script)
{
    //evaluate script:
    std::u16string u16name   = MU16StringFromU8(name  .c_str());
    std::u16string u16script = MU16StringFromU8(script.c_str());
    const wchar_t *c16name   = (const wchar_t *)u16name  .c_str();
    const wchar_t *c16script = (const wchar_t *)u16script.c_str();

    JsValueRef  result = JS_INVALID_REFERENCE;
    JsErrorCode error  = JsRunScript(c16script, ++mCodeId, c16name, &result);

    //print exception if needed:
    if (error == JsNoError)
    {
        return;
    }

    JsValueRef exception = JS_INVALID_REFERENCE;
    JsGetAndClearException(&exception);

    std::wstring u16message = L"JavaScript Exception\n";
    AppendExceptionInfo(&u16message, L"Type ", exception, L"name"   );
    AppendExceptionInfo(&u16message, L"Cause", exception, L"message");
    AppendExceptionInfo(&u16message, L"Stack", exception, L"stack"  );

    std::string message = MU8StringFromU16((const char16_t *)u16message.c_str());
    onException(message);
}

JsValueRef MWin32JsVM::onCallNativeFunction(
    JsValueRef callee, bool isConstruct, JsValueRef *args, USHORT argc, void *custom)
{
    //function.
    auto jsVM   = (MWin32JsVM *)instance();
    auto funcId = (intptr_t)custom;
    MGenericFunction::ptr func = jsVM->mNativeFunctions[funcId];

    //build argument:
    std::vector<reflect::any> cppArgs;

    //NOTE: first argument is "this".
    for (USHORT i = 1; i < argc; ++i)
    {
        reflect::any cppArg = MWin32JsObjectPool::instance()->getCppValue(args[i]);
        cppArgs.push_back(cppArg);
    }

    //call.
    reflect::any cppRet = func->call_with_args(cppArgs);

    //return.
    return MWin32JsObjectPool::instance()->getJsValue(cppRet);
}

void MWin32JsVM::AppendExceptionInfo(
    std::wstring *appended, const wchar_t *desc, JsValueRef obj, const wchar_t *keyName)
{
    JsValueRef key = JS_INVALID_REFERENCE;
    JsGetPropertyIdFromName(keyName, &key);

    JsValueRef value = JS_INVALID_REFERENCE;
    JsGetProperty(obj, key, &value);

    JsValueRef string = JS_INVALID_REFERENCE;
    JsConvertValueToString(value, &string);

    const wchar_t *chars = nullptr;
    size_t charsSize = 0;
    JsStringToPointer(string, &chars, &charsSize);

    appended->append(desc );
    appended->append(L": ");
    appended->append(chars);
    appended->append(L"\n");
}
