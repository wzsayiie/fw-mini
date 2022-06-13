#include "mwin32jsrt.h"
#include "mencode.h"

//assist:

static void AppendExceptionInfo(
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

minikit_class(MJsFunction, MBaseFunction)
{
public:
    MJsFunction(MWin32JsVM *jsVM, JsValueRef func)
    {
        JsAddRef(func, nullptr);

        mJsVM = jsVM;
        mFunc = func;
    }

    ~MJsFunction()
    {
        JsRelease(mFunc, nullptr);
    }

    void on_call() const override
    {
        //argument:
        std::vector<JsValueRef> jsArgs;

        //NOTE: first argument is "this".
        JsValueRef thisArg = JS_INVALID_REFERENCE;
        JsGetUndefinedValue(&thisArg);
        jsArgs.push_back(thisArg);

        auto count = reflect::get_arg_count();
        for (int i = 0; i < count; ++i)
        {
            reflect::any cppValue = reflect::get_arg_value(i);
            JsValueRef jsValue = mJsVM->getJsValue(cppValue);

            jsArgs.push_back(jsValue);
        }

        //call.
        JsValueRef jsRet = JS_INVALID_REFERENCE;
        JsCallFunction(mFunc, jsArgs.data(), (USHORT)jsArgs.size(), &jsRet);

        //return.
        reflect::any cppRet = mJsVM->getCppValue(jsRet);
        reflect::return_value(cppRet);
    }

private:
    MWin32JsVM *mJsVM;
    JsValueRef  mFunc;
};

//js vm:

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

JsValueRef MWin32JsVM::getJsValue(const reflect::any &cppValue)
{
    switch (cppValue.preferred_type())
    {
        case reflect::data_type::is_object:
        {
            JsValueRef jsValue = JS_INVALID_REFERENCE;
            JsCreateObject(&jsValue);

            //bind js and cpp object.
            JsSetObjectBeforeCollectCallback(jsValue, nullptr, onCollectJsObject);
            mObjectMap[jsValue] = cppValue;

            return jsValue;
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
            JsValueRef undefine = JS_INVALID_REFERENCE;
            JsGetUndefinedValue(&undefine);
            return undefine;
        }
    }
}

reflect::any MWin32JsVM::getCppValue(JsValueRef jsValue)
{
    JsValueType jsType = JsUndefined;
    JsGetValueType(jsValue, &jsType);

    switch (jsType)
    {
        case JsFunction:
        {
            return MJsFunction::create(this, jsValue);
        }

        case JsObject:
        {
            auto target = mObjectMap.find(jsValue);
            if (target != mObjectMap.end())
            {
                return target->second;
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

void MWin32JsVM::onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func)
{
    auto funcId = (int64_t)mNativeFunctions.size();
    mNativeFunctions[funcId] = func;

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
    auto jsVM = (MWin32JsVM *)instance();

    //function.
    auto funcId = (int64_t)custom;
    MBaseFunction::ptr func = jsVM->mNativeFunctions[funcId];

    //argument:
    std::vector<reflect::any> cppArgs;

    //NOTE: first argument is "this".
    for (USHORT i = 1; i < argc; ++i)
    {
        reflect::any cppArg = jsVM->getCppValue(args[i]);
        cppArgs.push_back(cppArg);
    }

    //call.
    reflect::any cppRet = func->call_with_args(cppArgs);

    //return.
    return jsVM->getJsValue(cppRet);
}

void MWin32JsVM::onCollectJsObject(JsRef value, void *custom)
{
    auto jsVM = (MWin32JsVM *)instance();
    jsVM->mObjectMap.erase(value);
}
