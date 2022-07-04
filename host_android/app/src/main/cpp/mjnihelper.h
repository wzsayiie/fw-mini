#pragma once

#include <jni.h>
#include "mobject.h"

//environment:

JNIEnv *MJniGetEnv();

#define mini_jni_env MJniGetEnv()

//global reference:

struct MJniGlobalJClassMaker {
    jclass operator<<(const char *classPath) const;
};

struct MJniGlobalJObjectMaker {
    std::shared_ptr<_jobject> operator<<(jobject ref) const;
};

#define mini_global_jclass  MJniGlobalJClassMaker ()<<
#define mini_global_jobject MJniGlobalJObjectMaker()<<

//type conversion:

struct MJniLocalJBytesMaker {
    jbyteArray operator<<(const MVector<uint8_t>::ptr &bytes) const;
};

struct MJniLocalJStringMaker {
    jstring operator<<(const std::string &str) const;
};

#define mini_local_jbytes  MJniLocalJBytesMaker ()<<
#define mini_local_jstring MJniLocalJStringMaker()<<

struct MJniCppBytesMaker {
    MVector<uint8_t>::ptr operator<<(jbyteArray jBytes) const;
};

struct MJniCppStringMaker {
    std::string operator<<(jstring jStr) const;
};

#define mini_cpp_bytes  MJniCppBytesMaker ()<<
#define mini_cpp_string MJniCppStringMaker()<<
