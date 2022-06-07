#pragma once

#include "mobject.h"

m_class(MJsVM, MObject) {
public:
    M_HOST_NEED_CALL static void setInstance(const MJsVM::ptr &obj);
    static MJsVM *instance();

public:
    virtual void setExceptionListener(const MBaseFunction::ptr &listener);
    virtual void registerFunction(const std::string &name, const MBaseFunction::ptr &func);
    virtual void evaluate(const std::string &name, const std::string &script);

private:
    static MJsVM::ptr sInstance;
};
