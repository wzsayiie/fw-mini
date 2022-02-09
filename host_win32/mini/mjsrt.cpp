#define USE_EDGEMODE_JSRT

#include "mjsrt.h"
#include <jsrt.h>
#include "mhostjs.h"

static def_singleton(sObjectMap, std::map<JsValueRef, MObject *>())

static JsRuntimeHandle sRuntime = JS_INVALID_RUNTIME_HANDLE;
static JsContextRef    sContext = JS_INVALID_REFERENCE;
static JsSourceContext sSource  = 0;

static void InitializeRuntime()
{
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &sRuntime);
    JsCreateContext(sRuntime, &sContext);
    JsSetCurrentContext(sContext);
}

static void CALLBACK JsObjectBeforeCollect(JsRef value, void *callbackState)
{
    MObject *object = sObjectMap()[value];
    sObjectMap().erase(value);

    //IMPORTANT: release the native object.
    MRelease(object);
}

static JsValueRef AddJsObjectFromObject(MObject *object)
{
    JsValueRef value = JS_INVALID_REFERENCE;
    JsCreateObject(&value);

    //set the flag.
    JsPropertyIdRef nativeId = JS_INVALID_REFERENCE;
    JsValueRef nativeValue = JS_INVALID_REFERENCE;
    JsGetPropertyIdFromName(L"isNativeObject", &nativeId);
    JsBoolToBoolean(true, &nativeValue);
    JsSetProperty(value, nativeId, nativeValue, true);
    
    sObjectMap()[value] = object;

    //IMPORTANT: retain one reference count of native object.
    MRetain(object);
    JsSetObjectBeforeCollectCallback(value, nullptr, JsObjectBeforeCollect);

    return value;
}

static MObject *GetObjectFromJsObject(JsValueRef value)
{
    auto iterator = sObjectMap().find(value);
    if (iterator != sObjectMap().end())
    {
        return iterator->second;
    }
    return nullptr;
}

static MLambda *CreateLambdaFromJsFunction(JsValueRef value)
{
    class JsFuncionWrapper : public MUnknown
    {
    public:
        JsFuncionWrapper(JsValueRef value)
        {
            JsAddRef(value, nullptr);
            mValue = value;
        }

        ~JsFuncionWrapper()
        {
            JsRelease(mValue, nullptr);
        }

        static void call(MObject *load)
        {
            JsValueRef thisArg = JS_INVALID_REFERENCE;
            JsGetUndefinedValue(&thisArg);

            auto wrapper = (JsFuncionWrapper *)load;
            JsCallFunction(wrapper->mValue, &thisArg, 1, nullptr);
        }

    private:
        JsValueRef mValue;
    };

    auto wrapper = new JsFuncionWrapper(value);
    return MLambdaCreate(JsFuncionWrapper::call, wrapper);
}

static MObject *CopyObjectFromJsValue(JsValueRef value)
{
    if (value == JS_INVALID_REFERENCE)
    {
        return nullptr;
    }

    JsValueType type = JsUndefined;
    JsGetValueType(value, &type);

    if (type == JsBoolean)
    {
        bool raw = false;
        JsBooleanToBool(value, &raw);
        return MBoolCreate(raw);
    }
    else if (type == JsNumber)
    {
        double raw = 0;
        JsNumberToDouble(value, &raw);
        return MDoubleCreate(raw);
    }
    else if (type == JsString)
    {
        const wchar_t *raw = nullptr;
        size_t rawSize = 0;
        JsStringToPointer(value, &raw, &rawSize);
        return MStringCreateU16((const char16_t *)raw);
    }
    else if (type == JsObject)
    {
        MObject *object = GetObjectFromJsObject(value);
        return MRetain(object);
    }
    else if (type == JsFunction)
    {
        return CreateLambdaFromJsFunction(value);
    }
    else
    {
        return nullptr;
    }
}

static JsValueRef JsValueFromObject(MObject *object)
{
    if (!object)
    {
        return JS_INVALID_REFERENCE;
    }

    MTypeId type = MGetTypeId(object);
    if (type == MTypeIdOf<MBool *>::Value)
    {
        bool raw = MBoolValue(object);

        JsValueRef value = JS_INVALID_REFERENCE;
        JsBoolToBoolean(raw, &value);
        return value;
    }
    else if (MIsNumberObject(type))
    {
        double raw = MDoubleValue(object);

        JsValueRef value = JS_INVALID_REFERENCE;
        JsDoubleToNumber(raw, &value);
        return value;
    }
    else if (type == MTypeIdOf<MString *>::Value)
    {
        auto raw     = (const wchar_t *)MStringU16Chars((MString *)object);
        auto rawSize = (size_t)MStringU16Size((MString *)object);

        JsValueRef value = JS_INVALID_REFERENCE;
        JsPointerToString(raw, rawSize, &value);
        return value;
    }
    else
    {
        return AddJsObjectFromObject(object);
    }
}

static JsValueRef CALLBACK NativeFunction(
    JsValueRef callee, bool isConstruct, JsValueRef *args, unsigned short argc, void *callbackState)
{
    MArrayRef params;

    //NOTE: the first argument is "this".
    if (argc > 1)
    {
        params = m_auto_release MArrayCreate();
        for (int index = 1; index < argc; ++index)
        {
            MObjectRef object = m_auto_release CopyObjectFromJsValue(args[index]);
            MArrayAppend(params.get(), object.get());
        }
    }

    auto funcName = (MString *)callbackState;
    MObjectRef returnObject = m_auto_release _MJsOnCallCopyRet(funcName, params.get());

    return JsValueFromObject(returnObject.get());
}

static void RegisterFunc(MString *name)
{
    //NOTE: intentional memory leak.
    //the value used as a custom state for the callback function.
    MRetain(name);

    JsValueRef funcObject = JS_INVALID_REFERENCE;
    JsCreateFunction(NativeFunction, name, &funcObject);

    JsValueRef propertyId = JS_INVALID_REFERENCE;
    JsGetPropertyIdFromName((const wchar_t *)MStringU16Chars(name), &propertyId);

    JsValueRef globalObject = JS_INVALID_REFERENCE;
    JsGetGlobalObject(&globalObject);
    JsSetProperty(globalObject, propertyId, funcObject, true);
}

static void AppendExceptionInfo(
    std::wstring *info, JsValueRef exception, const wchar_t *name, const wchar_t *desc)
{
    JsValueRef id = JS_INVALID_REFERENCE;
    JsGetPropertyIdFromName(name, &id);

    JsValueRef value = JS_INVALID_REFERENCE;
    JsGetProperty(exception, id, &value);

    JsValueRef string = JS_INVALID_REFERENCE;
    JsConvertValueToString(value, &string);

    const wchar_t *chars = nullptr;
    size_t charsSize = 0;
    JsStringToPointer(string, &chars, &charsSize);

    info->append(desc );
    info->append(L": ");
    info->append(chars);
    info->append(L"\n");
}

static void RunScript(MString *name, MString *script)
{
    //execute the script.
    auto scriptCode = (const wchar_t *)MStringU16Chars(script);
    auto scriptName = (const wchar_t *)MStringU16Chars(name);

    JsValueRef result = JS_INVALID_REFERENCE;
    JsErrorCode error = JsRunScript(scriptCode, ++sSource, scriptName, &result);

    //print runtime exception if need.
    if (error == JsNoError)
    {
        return;
    }

    JsValueRef exception = JS_INVALID_REFERENCE;
    JsGetAndClearException(&exception);

    std::wstring info = L"JavaScript Exception\n";
    AppendExceptionInfo(&info, exception, L"name"   , L"Type ");
    AppendExceptionInfo(&info, exception, L"message", L"Cause");
    AppendExceptionInfo(&info, exception, L"url"    , L"File ");
    AppendExceptionInfo(&info, exception, L"line"   , L"Line ");
    AppendExceptionInfo(&info, exception, L"stack"  , L"Stack");

    auto infoChars = (const char16_t *)info.c_str();
    MStringRef infoString = m_auto_release MStringCreateU16(infoChars);
    _MJsOnHappenError(infoString.get());
}

void MInstallJSRuntime()
{
    InitializeRuntime();

    _MJsSetRegisterFunc(RegisterFunc);
    _MJsSetRunScript(RunScript);
}
