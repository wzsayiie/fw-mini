#pragma once

#include "mfilemanager.h"
#include "mjnihelper.h"

m_class(MAndrdBundle, MBundle) {
protected:
    MVector<uint8_t>::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};

static jclass GetGlobalJClass() {
    static jclass global = m_global_jclass "src/app/mini/MAndrdBundle";
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
    return m_local_jstring MBundle::ResBundleDirectoryName;
}

MVector<uint8_t>::ptr MAndrdBundle::onLoadResource(const std::string &path) {
    static jmethodID method = m_jni_env->GetStaticMethodID(cls, __func__, "(Ljava/lang/String;)[B");

    jstring jPath = m_local_jstring path;
    auto    jRes  = (jbyteArray)m_jni_env->CallStaticObjectMethod(cls, method, jPath);

    return m_cpp_bytes jRes;
}

std::string MAndrdBundle::onGetResBundleDirectory() {
    static jmethodID method = m_jni_env->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    return m_cpp_string (jstring)m_jni_env->CallStaticObjectMethod(cls, method);
}

std::string MAndrdBundle::onGetDocumentDirectory() {
    static jmethodID method = m_jni_env->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    return m_cpp_string (jstring)m_jni_env->CallStaticObjectMethod(cls, method);
}

std::string MAndrdBundle::onGetTemporaryDirectory() {
    static jmethodID method = m_jni_env->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    return m_cpp_string (jstring)m_jni_env->CallStaticObjectMethod(cls, method);
}
