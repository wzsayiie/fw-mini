#pragma once

#include <jni.h>
#include "mobject.h"

jclass  MJNINewGlobalJClass(const char *clsPath);
JNIEnv *MJNIGetEnv();

jbyteArray MJNINewLocalJBytes (const MVector<uint8_t>::ptr &bytes);
jstring    MJNINewLocalJString(const std::string &str);

MVector<uint8_t>::ptr MJNIGetBytes (jbyteArray jBytes);
std::string           MJNIGetString(jstring jStr);
