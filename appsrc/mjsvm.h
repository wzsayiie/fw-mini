#pragma once

#include "mgeneric.h"
#include "mobject.h"

m_class(MJsVM, MObject) {
public:
    M_HOST_CALL static void setInstance(const MJsVM::ptr &obj);
    static MJsVM *instance();

public:
    void setExceptionListener(const MGenericFunction::ptr &listener);
    MGenericFunction::ptr exceptionListener();

    void registerFunction(const std::string &name, const MGenericFunction::ptr &func);
    void evaluate(const std::string &name, const std::string &script);

protected:
    M_HOST_CALL void onException(const std::string &message);

    M_HOST_IMPL virtual
        void onRegisterFunction(const std::string &name, const MGenericFunction::ptr &func);

    M_HOST_IMPL virtual
        void onEvaluate(const std::string &name, const std::string &script);

private:
    static MJsVM::ptr sIntance;

private:
    MGenericFunction::ptr mExceptionListener;
};
