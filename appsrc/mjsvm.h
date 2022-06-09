#pragma once

#include "mobject.h"

//js vm:

m_class(MJsVM, MObject) {
public:
    static MJsVM *instance();

public:
    void setExceptionListener(const MBaseFunction::ptr &listener);
    void registerFunction(const std::string &name, const MBaseFunction::ptr &func);
    void evaluate(const std::string &name, const std::string &script);
};

//virtual js vm:

m_class(MVirtualJsVM, MObject) {
public:
    M_HOST_NEED_CALL static void setInstance(const MVirtualJsVM::ptr &obj);
    static MVirtualJsVM *instance();

public:
    virtual void setExceptionListener(const MBaseFunction::ptr &listener);
    virtual void registerFunction(const std::string &name, const MBaseFunction::ptr &func);
    virtual void evaluate(const std::string &name, const std::string &script);

private:
    static MVirtualJsVM::ptr sInstance;
};
