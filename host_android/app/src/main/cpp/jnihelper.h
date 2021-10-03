#pragma once

#include <jni.h>
#include "mbasics.h"

MString *MStringCopyJObject(JNIEnv *env, jstring    src);
MData   *MDataCopyJObject  (JNIEnv *env, jbyteArray src);

jstring    JNewString   (JNIEnv *env, MString *src);
jbyteArray JNewByteArray(JNIEnv *env, MData   *src);

struct JLocalRefDeleteHelper
{
private:
    JNIEnv *mJniEnv = nullptr;

public:
    JLocalRefDeleteHelper(JNIEnv *env)
    {
        mJniEnv = env;
    }

    template<typename T> std::shared_ptr<T> operator<<(T *object)
    {
        JNIEnv *env = mJniEnv;
        return std::shared_ptr<T>(object, [env](T *a) { env->DeleteLocalRef(a); });
    }
};
#define j_auto_delete(env) JLocalRefDeleteHelper(env)<<
