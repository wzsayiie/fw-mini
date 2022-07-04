#include "mjnihelper.h"

//environment:

const jint UsingVersion = JNI_VERSION_1_6;
static JNIEnv *sEnv = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *) {
    vm->GetEnv((void **)&sEnv, UsingVersion);
    return UsingVersion;
}

JNIEnv *MJniGetEnv() {
    return sEnv;
}

//global reference:

jclass MJniGlobalJClassMaker::operator<<(const char *classPath) const {
    jclass local = sEnv->FindClass(classPath);
    return (jclass)sEnv->NewGlobalRef(local);
}

std::shared_ptr<_jobject> MJniGlobalJObjectMaker::operator<<(jobject ref) const {
    if (!ref) {
        return nullptr;
    }

    jobject global = sEnv->NewGlobalRef(ref);
    return {
        global, [](jobject stored) {
            sEnv->DeleteGlobalRef(stored);
        }
    };
}

//type conversion:

jbyteArray MJniLocalJBytesMaker::operator<<(const MVector<uint8_t>::ptr &bytes) const {
    if (!bytes || bytes->empty()) {
        return nullptr;
    }

    auto ptr = (jbyte *)bytes->data();
    auto len = (jsize  )bytes->size();

    jbyteArray jBytes = sEnv->NewByteArray(len);
    sEnv->SetByteArrayRegion(jBytes, 0, len, ptr);

    return jBytes;
}

jstring MJniLocalJStringMaker::operator<<(const std::string &str) const {
    if (str.empty()) {
        return nullptr;
    }

    return sEnv->NewStringUTF(str.c_str());
}

MVector<uint8_t>::ptr MJniCppBytesMaker::operator<<(jbyteArray jBytes) const {
    if (!jBytes) {
        return nullptr;
    }

    auto ptr = (uint8_t *)sEnv->GetByteArrayElements(jBytes, nullptr);
    auto len = (size_t   )sEnv->GetArrayLength(jBytes);

    auto bytes = MVector<uint8_t>::create(ptr, ptr + len);
    sEnv->ReleaseByteArrayElements(jBytes, (jbyte *)ptr, 0);

    return bytes;
}

std::string MJniCppStringMaker::operator<<(jstring jStr) const {
    if (!jStr) {
        return "";
    }

    const char *chars = sEnv->GetStringUTFChars(jStr, nullptr);

    std::string str(chars ? chars : "");
    sEnv->ReleaseStringUTFChars(jStr, chars);

    return str;
}
