#pragma once

#include "mfilemanager.h"
#include "mjnihelper.h"

const char *MAndrdBundleJClassPath = "src/app/mini/MAndrdBundle";

minikit_class(MAndrdBundle, MBundle) {
protected:
    MVector<uint8_t>::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
};

extern "C" JNIEXPORT void JNICALL
Java_src_app_mini_MAndrdBundle_installNative(JNIEnv *, jclass) {
    auto obj = MAndrdBundle::create();
    MBundle::setInstance(obj);
}

extern "C" JNIEXPORT jstring JNICALL
Java_src_app_mini_MAndrdBundle_resBundleName(JNIEnv *, jclass) {
    std::string name = MBundle::ResBundleDirectoryName;
    return MJNINewLocalJString(name);
}

MVector<uint8_t>::ptr MAndrdBundle::onLoadResource(const std::string &path) {
    static jclass    cls = MJNINewGlobalJClass(MAndrdBundleJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "(Ljava/lang/String;)[B");

    auto jPath  = MJNINewLocalJString(path);
    auto jBytes = (jbyteArray)MJNIGetEnv()->CallStaticObjectMethod(cls, mtd, jPath);

    return MJNIGetBytes(jBytes);
}

std::string MAndrdBundle::onGetResBundleDirectory() {
    static jclass    cls = MJNINewGlobalJClass(MAndrdBundleJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    auto jDir = (jstring)MJNIGetEnv()->CallStaticObjectMethod(cls, mtd);

    return MJNIGetString(jDir);
}

std::string MAndrdBundle::onGetDocumentDirectory() {
    static jclass    cls = MJNINewGlobalJClass(MAndrdBundleJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    auto jDir = (jstring)MJNIGetEnv()->CallStaticObjectMethod(cls, mtd);

    return MJNIGetString(jDir);
}

std::string MAndrdBundle::onGetTemporaryDirectory() {
    static jclass    cls = MJNINewGlobalJClass(MAndrdBundleJClassPath);
    static jmethodID mtd = MJNIGetEnv()->GetStaticMethodID(cls, __func__, "()Ljava/lang/String;");

    auto jDir = (jstring)MJNIGetEnv()->CallStaticObjectMethod(cls, mtd);

    return MJNIGetString(jDir);
}
