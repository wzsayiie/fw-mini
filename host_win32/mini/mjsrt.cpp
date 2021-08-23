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

static MObject *CopyObjectFromJsValue(JsValueRef value) {
    if (value == JS_INVALID_REFERENCE) {
        return nullptr;
    }

    JsValueType type = JsUndefined;
    JsGetValueType(value, &type);

    switch (type) {
        case JsBoolean: {
            bool raw = false;
            JsBooleanToBool(value, &raw);
            return MBoolCreate(raw);
        }
        case JsNumber: {
            double raw = 0;
            JsNumberToDouble(value, &raw);
            return MFloatCreate((float)raw);
        }
        case JsString: {
            const wchar_t *raw = nullptr;
            size_t size = 0;
            JsStringToPointer(value, &raw, &size);
            return MStringCreateU16((const char16_t *)raw);
        }
        case JsObject: {
            return nullptr;
        }
        default: {
            return nullptr;
        }
    }
}

static JsValueRef JsValueFromObject(MObject *object) {
    if (!object) {
        return JS_INVALID_REFERENCE;
    }

    switch (MGetType(object)) {
        case MType_MBool: {
            JsValueRef value = JS_INVALID_REFERENCE;
            JsBoolToBoolean(MBoolValue((MBool *)object), &value);
            return value;
        }
        case MType_MInt: {
            JsValueRef value = JS_INVALID_REFERENCE;
            JsIntToNumber(MIntValue((MInt *)object), &value);
            return value;
        }
        case MType_MFloat: {
            JsValueRef value = JS_INVALID_REFERENCE;
            JsDoubleToNumber(MFloatValue((MFloat *)object), &value);
            return value;
        }
        case MType_MString: {
            auto raw  = (const wchar_t *)MStringU16Chars((MString *)object);
            auto size = (size_t)MStringU16Size((MString *)object);

            JsValueRef value = JS_INVALID_REFERENCE;
            JsPointerToString(raw, size, &value);
            return value;
        }
        default: {
            return JS_INVALID_REFERENCE;
        }
    }
}

static JsValueRef CALLBACK NativeFunction(
    JsValueRef callee, bool isConstruct, JsValueRef *args, unsigned short argc, void *callbackState)
{
    MArray *params = nullptr;

    //NOTE: the first argument is "this".
    if (argc > 1)
    {
        params = MArrayCreate();
        for (int index = 1; index < argc; ++index)
        {
            MObjectRef object = m_auto_release CopyObjectFromJsValue(args[index]);
            MArrayAppend(params, object.get());
        }
    }

    auto funcName = (MString *)callbackState;
    MObjectRef returnObject = m_auto_release _MJsOnCallCopyRet(funcName, params);

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

    //print runtime exception if needed.
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
    _MjsSetRunScript(RunScript);
}
