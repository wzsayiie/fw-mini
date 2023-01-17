#include "MPROFILE.h"
#include "mbundle.h"
#include "mjnihelper.h"

m_class(MAndrdBundle, MBundle) {
protected:
    MData::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};

static jclass C() {
    static std::shared_ptr<_jclass> cls = m_global_jclass << "src/app/mini/MAndrdBundle";
    return cls.get();
}

extern "C" JNIEXPORT JNICALL void
Java_src_app_mini_MAndrdBundle_installNative(JNIEnv *, jclass) {
    auto obj = MAndrdBundle::create();
    MBundle::setInstance(obj);
}

extern "C" JNIEXPORT JNICALL jstring
Java_src_app_mini_MAndrdBundle_resBundleName(JNIEnv *, jclass) {
    return m_local_jstring << mp_resbundle_folder_name;
}

MData::ptr MAndrdBundle::onLoadResource(const std::string &path) {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "(Ljava/lang/String;)[B");

    jstring jPath = m_local_jstring << path;
    jobject jRes  = m_jenv->CallStaticObjectMethod(C(), method, jPath);

    return m_cpp_bytes << jRes;
}

std::string MAndrdBundle::onGetResBundleDirectory() {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "()Ljava/lang/String;");

    return m_cpp_string << m_jenv->CallStaticObjectMethod(C(), method);
}

std::string MAndrdBundle::onGetDocumentDirectory() {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "()Ljava/lang/String;");

    return m_cpp_string << m_jenv->CallStaticObjectMethod(C(), method);
}

std::string MAndrdBundle::onGetTemporaryDirectory() {
    static jmethodID method = m_jenv->GetStaticMethodID(C(), __func__, "()Ljava/lang/String;");

    return m_cpp_string << m_jenv->CallStaticObjectMethod(C(), method);
}
