#include "jnihelper.h"

MString *MStringCopyJString(JNIEnv *env, jstring src)
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

MData *MDataCopyJByteArray(JNIEnv *env, jbyteArray src)
{
    if (!src)
    {
        return nullptr;
    }

    jbyte *bytes = env->GetByteArrayElements(src, nullptr);
    jsize  size  = env->GetArrayLength(src);

    MData *dst = MDataCopy((const uint8_t *)bytes, (int)size);
    env->ReleaseByteArrayElements(src, bytes, 0);

    return dst;
}

MData *MDataCopyJIntArray(JNIEnv *env, jintArray src)
{
    if (!src)
    {
        return nullptr;
    }

    jint *ints = env->GetIntArrayElements(src, nullptr);
    jsize size = env->GetArrayLength(src);

    MData *dst = MDataCopy((const uint8_t *)ints, (int)size * 4);
    env->ReleaseIntArrayElements(src, ints, 0);

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

jintArray JNewIntArray(JNIEnv *env, MData *src)
{
    if (!src)
    {
        return nullptr;
    }

    auto ints = (const jint *)MDataBytes(src);
    auto size = (jsize)MDataSize(src) / 4;

    jintArray dst = env->NewIntArray(size);
    env->SetIntArrayRegion(dst, 0, size, ints);

    return dst;
}
