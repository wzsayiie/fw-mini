#include "mjnihelper.h"

const jint UsingVersion = JNI_VERSION_1_6;
static JNIEnv *sEnv = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *) {
    vm->GetEnv((void **)&sEnv, UsingVersion);
    return UsingVersion;
}

jclass MJNINewGlobalJClass(const char *clsPath) {
    jclass localRef = sEnv->FindClass(clsPath);
    return (jclass)sEnv->NewGlobalRef(localRef);
}

JNIEnv *MJNIGetEnv() {
    return sEnv;
}

jbyteArray MJNINewLocalJBytes(const MVector<uint8_t>::ptr &bytes) {
    if (!bytes || bytes->empty()) {
        return nullptr;
    }

    auto ptr = (jbyte *)bytes->data();
    auto len = (jsize  )bytes->size();

    jbyteArray jBytes = sEnv->NewByteArray(len);
    sEnv->SetByteArrayRegion(jBytes, 0, len, ptr);

    return jBytes;
}

jstring MJNINewLocalJString(const std::string &str) {
    if (str.empty()) {
        return nullptr;
    }

    return sEnv->NewStringUTF(str.c_str());
}

MVector<uint8_t>::ptr MJNIGetBytes(jbyteArray jBytes) {
    if (!jBytes) {
        return nullptr;
    }

    auto ptr = (uint8_t *)sEnv->GetByteArrayElements(jBytes, nullptr);
    auto len = (size_t   )sEnv->GetArrayLength(jBytes);

    auto bytes = MVector<uint8_t>::create(ptr, ptr + len);
    sEnv->ReleaseByteArrayElements(jBytes, (jbyte *)ptr, 0);

    return bytes;
}

std::string MJNIGetString(jstring jStr) {
    if (!jStr) {
        return "";
    }

    const char *chars = sEnv->GetStringUTFChars(jStr, nullptr);

    std::string str(chars ? chars : "");
    sEnv->ReleaseStringUTFChars(jStr, chars);

    return str;
}
