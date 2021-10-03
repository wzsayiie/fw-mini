#include "jnihelper.h"

MString *MStringCopyJObject(JNIEnv *env, jstring src)
{
    if (!src)
    {
        return nullptr;
    }

    //NOTE: the return string of GetStringChars has no terminator '\0'.
    const jchar *chars  = env->GetStringChars(src, nullptr);
    jsize        length = env->GetStringLength(src);

    MString *dst = MStringCopyU16((const char16_t *)chars, (int)length);
    env->ReleaseStringChars(src, chars);

    return dst;
}

MData *MDataCopyJObject(JNIEnv *env, jbyteArray src)
{
    if (!src)
    {
        return nullptr;
    }

    jbyte *bytes = env->GetByteArrayElements(src, nullptr);
    jsize  size  = env->GetArrayLength(src);

    MData *dst = MDataCreate((const uint8_t *)bytes, (int)size);
    env->ReleaseByteArrayElements(src, bytes, 0);

    return dst;
}

jstring JNewString(JNIEnv *env, MString *src)
{
    if (!src)
    {
        return nullptr;
    }

    auto chars = (const jchar *)MStringU16Chars(src);
    auto size  = (jsize)MStringU16Size(src);

    return env->NewString(chars, size);
}

jbyteArray JNewByteArray(JNIEnv *env, MData *src)
{
    if (!src)
    {
        return nullptr;
    }

    auto bytes = (const jbyte *)MDataBytes(src);
    auto size  = (jsize)MDataSize(src);

    jbyteArray dst = env->NewByteArray(size);
    env->SetByteArrayRegion(dst, 0, size, bytes);

    return dst;
}
