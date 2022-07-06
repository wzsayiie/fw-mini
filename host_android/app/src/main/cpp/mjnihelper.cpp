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

    auto byteBuf = (jbyte *)bytes->data();
    auto byteNum = (jsize  )bytes->size();

    jbyteArray jBytes = sEnv->NewByteArray(byteNum);
    sEnv->SetByteArrayRegion(jBytes, 0, byteNum, byteBuf);

    return jBytes;
}

jintArray MJniLocalJIntsMaker::operator<<(const MVector<int>::ptr &bytes) const {
    if (!bytes || bytes->empty()) {
        return nullptr;
    }

    auto intBuf = (jint *)bytes->data();
    auto intNum = (jsize )bytes->size();

    jintArray jInts = sEnv->NewIntArray(intNum);
    sEnv->SetIntArrayRegion(jInts, 0, intNum, intBuf);

    return jInts;
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

    auto byteBuf = (uint8_t *)sEnv->GetByteArrayElements(jBytes, nullptr);
    auto byteNum = (size_t   )sEnv->GetArrayLength(jBytes);

    auto bytes = MVector<uint8_t>::create(byteBuf, byteBuf + byteNum);
    sEnv->ReleaseByteArrayElements(jBytes, (jbyte *)byteBuf, 0);

    return bytes;
}

MVector<uint8_t>::ptr MJniCppBytesMaker::operator<<(jintArray jInts) const {
    if (!jInts) {
        return nullptr;
    }

    auto byteBuf = (uint8_t *)sEnv->GetIntArrayElements(jInts, nullptr);
    auto intNum  = (size_t   )sEnv->GetArrayLength(jInts);
    auto byteNum = intNum * sizeof(jint);

    auto bytes = MVector<uint8_t>::create(byteBuf, byteBuf + byteNum);
    sEnv->ReleaseIntArrayElements(jInts, (jint *)byteBuf, 0);

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
