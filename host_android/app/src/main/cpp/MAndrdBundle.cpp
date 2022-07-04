#pragma once

#include "mfilemanager.h"
#include "mjnihelper.h"

minikit_class(MAndrdBundle, MBundle) {
protected:
    MVector<uint8_t>::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};

static jclass GetGlobalJClass() {
    static jclass global = mini_global_jclass "src/app/mini/MAndrdBundle";
    return global;
}
#define cls GetGlobalJClass()

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdBundle_installNative(JNIEnv *, jclass) {
    auto obj = MAndrdBundle::create();
    MBundle::setInstance(obj);
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_MAndrdBundle_resBundleName(JNIEnv *, jclass) {
    return mini_local_jstring MBundle::ResBundleDirectoryName;
}

MVector<uint8_t>::ptr MAndrdBundle::onLoadResource(const std::string &path) {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "(Ljava/lang/String;)[B");

    jstring jPath = mini_local_jstring path;
    auto    jRes  = (jbyteArray)mini_jni_env->CallStaticObjectMethod(cls, method, jPath);

    return mini_cpp_bytes jRes;
}

std::string MAndrdBundle::onGetResBundleDirectory() {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    return mini_cpp_string (jstring)mini_jni_env->CallStaticObjectMethod(cls, method);
}

std::string MAndrdBundle::onGetDocumentDirectory() {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    return mini_cpp_string (jstring)mini_jni_env->CallStaticObjectMethod(cls, method);
}

std::string MAndrdBundle::onGetTemporaryDirectory() {
    static jmethodID method = mini_jni_env->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    return mini_cpp_string (jstring)mini_jni_env->CallStaticObjectMethod(cls, method);
}
