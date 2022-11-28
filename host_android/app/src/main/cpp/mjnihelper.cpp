#include "mjnihelper.h"

//environment:

const jint UsingVersion = JNI_VERSION_1_6;
static JNIEnv *sEnv = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *) {
    vm->GetEnv((void **)&sEnv, UsingVersion);
    return UsingVersion;
}

JNIEnv *_m_jenv_maker::operator->() const {
    return sEnv;
}

//global reference:

std::shared_ptr<_jclass> _m_global_jclass_maker::operator<<(const char *path) const {
    jclass local = sEnv->FindClass(path);

    auto global = (jclass)sEnv->NewGlobalRef(local);
    return std::shared_ptr<_jclass>(global, [](jclass stored) {
        sEnv->DeleteGlobalRef(stored);
    });
}

std::shared_ptr<_jobject> _m_global_jobject_maker::operator<<(jobject ref) const {
    if (!ref) {
        return nullptr;
    }

    jobject global = sEnv->NewGlobalRef(ref);
    return std::shared_ptr<_jobject>(global, [](jobject stored) {
        sEnv->DeleteGlobalRef(stored);
    });
}

//type conversion:

jbyteArray _m_local_jbytes_maker::operator<<(const MData::ptr &data) const {
    if (!data || data->empty()) {
        return nullptr;
    }

    auto byteBuf = (jbyte *)data->bytes ();
    auto byteNum = (jsize  )data->length();

    jbyteArray jBytes = sEnv->NewByteArray(byteNum);
    sEnv->SetByteArrayRegion(jBytes, 0, byteNum, byteBuf);

    return jBytes;
}

jintArray _m_local_jints_maker::operator<<(const std::vector<int> &data) const {
    if (data.empty()) {
        return nullptr;
    }

    auto intBuf = (jint *)data.data();
    auto intNum = (jsize )data.size();

    jintArray jInts = sEnv->NewIntArray(intNum);
    sEnv->SetIntArrayRegion(jInts, 0, intNum, intBuf);

    return jInts;
}

jstring _m_local_jstring_maker::operator<<(const std::string &str) const {
    if (str.empty()) {
        return nullptr;
    }

    return sEnv->NewStringUTF(str.c_str());
}

MData::ptr _m_cpp_bytes_maker::operator<<(jobject jObj) const {
    if (!jObj) {
        return nullptr;
    }

    auto jBytes  = (jbyteArray)jObj;
    auto byteBuf = (uint8_t * )sEnv->GetByteArrayElements(jBytes, nullptr);
    auto byteNum = (size_t    )sEnv->GetArrayLength(jBytes);

    auto bytes = MData::create(byteBuf, byteBuf + byteNum);
    sEnv->ReleaseByteArrayElements(jBytes, (jbyte *)byteBuf, 0);

    return bytes;
}

MData::ptr _m_cpp_ints_maker::operator<<(jobject jObj) const {
    if (!jObj) {
        return nullptr;
    }

    auto jInts   = (jintArray)jObj;
    auto byteBuf = (uint8_t *)sEnv->GetIntArrayElements(jInts, nullptr);
    auto intNum  = (size_t   )sEnv->GetArrayLength(jInts);
    auto byteNum = intNum * sizeof(jint);

    auto bytes = MData::create(byteBuf, byteBuf + byteNum);
    sEnv->ReleaseIntArrayElements(jInts, (jint *)byteBuf, 0);

    return bytes;
}

std::string _m_cpp_string_maker::operator<<(jobject jObj) const {
    if (!jObj) {
        return "";
    }

    auto jStr = (jstring)jObj;
    const char *chars = sEnv->GetStringUTFChars(jStr, nullptr);

    std::string str(chars ? chars : "");
    sEnv->ReleaseStringUTFChars(jStr, chars);

    return str;
}
