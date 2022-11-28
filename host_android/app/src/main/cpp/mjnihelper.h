#pragma once

#include <jni.h>
#include "mdata.h"

//environment:

struct _m_jenv_maker {
    JNIEnv *operator->() const;
};

const _m_jenv_maker m_jenv;

//global reference:

struct _m_global_jclass_maker {
    std::shared_ptr<_jclass> operator<<(const char *path) const;
};

struct _m_global_jobject_maker {
    std::shared_ptr<_jobject> operator<<(jobject ref) const;
};

const _m_global_jclass_maker  m_global_jclass ;
const _m_global_jobject_maker m_global_jobject;

//type conversion:

struct _m_local_jbytes_maker {
    jbyteArray operator<<(const MData::ptr &data) const;
};

struct _m_local_jints_maker {
    jintArray operator<<(const std::vector<int> &data) const;
};

struct _m_local_jstring_maker {
    jstring operator<<(const std::string &str) const;
};

const _m_local_jbytes_maker  m_local_jbytes ;
const _m_local_jints_maker   m_local_jints  ;
const _m_local_jstring_maker m_local_jstring;

struct _m_cpp_bytes_maker {
    MData::ptr operator<<(jobject jObj) const;
};

struct _m_cpp_ints_maker {
    MData::ptr operator<<(jobject jObj) const;
};

struct _m_cpp_string_maker {
    std::string operator<<(jobject jObj) const;
};

const _m_cpp_bytes_maker  m_cpp_bytes ;
const _m_cpp_ints_maker   m_cpp_ints  ;
const _m_cpp_string_maker m_cpp_string;
