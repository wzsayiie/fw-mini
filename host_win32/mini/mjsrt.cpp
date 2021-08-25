#define USE_EDGEMODE_JSRT

#include "mjsrt.h"
#include <jsrt.h>
#include "mhostjs.h"

static JsRuntimeHandle sRuntime = JS_INVALID_RUNTIME_HANDLE;
static JsContextRef    sContext = JS_INVALID_REFERENCE;
static JsSourceContext sSource  = 0;

static void InitializeRuntime()
{
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &sRuntime);
    JsCreateContext(sRuntime, &sContext);
    JsSetCurrentContext(sContext);
}

static JsValueRef GetJsObjectFromObject(MObject *object)
{
    //| return {
    //|   _mIsNative: true,
    //|   _mPtrHigh : xxxx,
    //|   _mPtrLow  : xxxx
    //| }
    //

    JsValueRef value = JS_INVALID_REFERENCE;
    JsCreateObject(&value);

    MJsPtrPattern pattern = { object };

    //set "_mIsNative":
    {
        JsPropertyIdRef id = JS_INVALID_REFERENCE;
        JsGetPropertyIdFromName(L"_mIsNative", &id);

        JsValueRef member = JS_INVALID_REFERENCE;
        JsBoolToBoolean(true, &member);

        JsSetProperty(value, id, member, true);
    }

    //set "_mPtrHigh":
    {
        JsPropertyIdRef id = JS_INVALID_REFERENCE;
        JsGetPropertyIdFromName(L"_mPtrHigh", &id);

        JsValueRef member = JS_INVALID_REFERENCE;
        JsIntToNumber(pattern.high, &member);

        JsSetProperty(value, id, member, true);
    }

    //set "_mPtrLow":
    {
        JsPropertyIdRef id = JS_INVALID_REFERENCE;
        JsGetPropertyIdFromName(L"_mPtrLow", &id);

        JsValueRef member = JS_INVALID_REFERENCE;
        JsIntToNumber(pattern.low, &member);

        JsSetProperty(value, id, member, true);
    }

    return value;
}

static MObject *GetObjectFromJsObject(JsValueRef value)
{
    //get "_mIsNative":
    bool isNative = false;
    {
        JsPropertyIdRef id = JS_INVALID_REFERENCE;
        JsGetPropertyIdFromName(L"_mIsNative", &id);

        JsValueRef member = JS_INVALID_REFERENCE;
        JsGetProperty(value, id, &member);

        JsBooleanToBool(member, &isNative);
    }
    if (!isNative)
    {
        return nullptr;
    }

    MJsPtrPattern pattern;

    //get "_mPtrHigh":
    {
        JsPropertyIdRef id = JS_INVALID_REFERENCE;
        JsGetPropertyIdFromName(L"_mPtrHigh", &id);

        JsValueRef member = JS_INVALID_REFERENCE;
        JsGetProperty(value, id, &member);

        JsNumberToInt(member, &pattern.high);
    }

    //get "_mPtrLow":
    {
        JsPropertyIdRef id = JS_INVALID_REFERENCE;
        JsGetPropertyIdFromName(L"_mPtrLow", &id);

        JsValueRef member = JS_INVALID_REFERENCE;
        JsGetProperty(value, id, &member);

        JsNumberToInt(member, &pattern.low);
    }

    return pattern.object;
}

static void CALLBACK JsObjectBeforeCollect(JsRef value, void *callbackState)
{
    MObject *object = GetObjectFromJsObject(value);
    MRelease(object);
}

static MObject *CopyObjectFromJsValue(JsValueRef value)
{
    if (value == JS_INVALID_REFERENCE)
    {
        return nullptr;
    }

    JsValueType type = JsUndefined;
    JsGetValueType(value, &type);

    switch (type)
    {
        case JsBoolean:
        {
            bool raw = false;
            JsBooleanToBool(value, &raw);
            return MBoolCreate(raw);
        }
        case JsNumber:
        {
            double raw = 0;
            JsNumberToDouble(value, &raw);

            if ((int64_t)raw < raw)
            {
                return MFloatCreate((float)raw);
            }
            else
            {
                return MIntCreate((int)raw);
            }
        }
        case JsString:
        {
            const wchar_t *raw = nullptr;
            size_t rawSize = 0;
            JsStringToPointer(value, &raw, &rawSize);
            return MStringCreateU16((const char16_t *)raw);
        }
        case JsObject:
        {
            MObject *object = GetObjectFromJsObject(value);
            return MRetain(object);
        }
        default:
        {
            return nullptr;
        }
    }
}

static JsValueRef JsValueFromObject(MObject *object) {
    if (!object)
    {
        return JS_INVALID_REFERENCE;
    }

    switch (MGetType(object))
    {
        case MType_MBool:
        {
            bool raw = MBoolValue((MBool *)object);

            JsValueRef value = JS_INVALID_REFERENCE;
            JsBoolToBoolean(raw, &value);
            return value;
        }
        case MType_MInt:
        {
            int raw = MIntValue((MInt *)object);

            JsValueRef value = JS_INVALID_REFERENCE;
            JsIntToNumber(raw, &value);
            return value;
        }
        case MType_MFloat:
        {
            float raw = MFloatValue((MFloat *)object);

            JsValueRef value = JS_INVALID_REFERENCE;
            JsDoubleToNumber(raw, &value);
            return value;
        }
        case MType_MString:
        {
            auto raw     = (const wchar_t *)MStringU16Chars((MString *)object);
            auto rawSize = (size_t)MStringU16Size((MString *)object);

            JsValueRef value = JS_INVALID_REFERENCE;
            JsPointerToString(raw, rawSize, &value);
            return value;
        }
        default:
        {
            JsValueRef value = GetJsObjectFromObject(object);

            //IMPORTANT: the js object will hold one reference count of the native object.
            MRetain(object);
            JsSetObjectBeforeCollectCallback(value, nullptr, JsObjectBeforeCollect);

            return value;
        }
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

static void AppendExceptionInfo(std::wstring *info, JsValueRef exception, const wchar_t *name)
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

    info->append(name );
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

    std::wstring info = L"JavaScript Exception:\n";
    AppendExceptionInfo(&info, exception, L"message");
    AppendExceptionInfo(&info, exception, L"url"    );
    AppendExceptionInfo(&info, exception, L"line"   );
    AppendExceptionInfo(&info, exception, L"stack"  );

    auto infoChars = (const char16_t *)info.c_str();
    MStringRef infoString = m_auto_release MStringCreateU16(infoChars);
    _MJsOnHappenError(infoString.get());
}

void MRegisterJSRT()
{
    InitializeRuntime();

    _MJsSetRegisterFunc(RegisterFunc);
    _MJsSetRunScript(RunScript);
}
