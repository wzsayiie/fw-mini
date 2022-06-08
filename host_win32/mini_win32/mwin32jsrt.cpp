#include "mwin32jsrt.h"
#include "mencode.h"

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
    JsSetCurrentContext(nullptr);
    JsRelease(mContext, nullptr);
    JsDisposeRuntime(mRuntime);

    mRuntime = JS_INVALID_RUNTIME_HANDLE;
    mContext = JS_INVALID_REFERENCE;
    mSource  = 0;
}

void MWin32JsVM::setExceptionListener(const MBaseFunction::ptr &listener)
{
    mExceptionListener = listener;
}

void MWin32JsVM::registerFunction(const std::string &name, const MBaseFunction::ptr &func)
{
}

void MWin32JsVM::evaluate(const std::string &name, const std::string &script)
{
    std::u16string u16name     = MU16StringFromU8(name  .c_str());
    std::u16string u16script   = MU16StringFromU8(script.c_str());
    const wchar_t *nameChars   = (const wchar_t *)u16name  .c_str();
    const wchar_t *scriptChars = (const wchar_t *)u16script.c_str();

    JsValueRef  result = JS_INVALID_REFERENCE;
    JsErrorCode error  = JsRunScript(scriptChars, ++mSource, nameChars, &result);

    if (error == JsNoError)
    {
        return;
    }
    if (!mExceptionListener)
    {
        return;
    }

    JsValueRef exception = JS_INVALID_REFERENCE;
    JsGetAndClearException(&exception);

    std::wstring message = L"JavaScript Exception\n";
    AppendExceptionInfo(&message, L"Type ", exception, L"name"   );
    AppendExceptionInfo(&message, L"Cause", exception, L"message");
    AppendExceptionInfo(&message, L"File ", exception, L"url"    );
    AppendExceptionInfo(&message, L"Line ", exception, L"line"   );
    AppendExceptionInfo(&message, L"Stack", exception, L"stack"  );

    mExceptionListener->call_with_args({ (const char16_t *)message.c_str() });
}
