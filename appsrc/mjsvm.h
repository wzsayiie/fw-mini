#pragma once

#include "mobject.h"

minikit_class(MJsVM, MObject) {
public:
    MINIKIT_HOST_CALL static void setInstance(const MJsVM::ptr &obj);
    static MJsVM *instance();

public:
    void setExceptionListener(const MBaseFunction::ptr &listener);
    MBaseFunction::ptr exceptionListener();

    void registerFunction(const std::string &name, const MBaseFunction::ptr &func);
    void evaluate(const std::string &name, const std::string &script);

protected:
    MINIKIT_HOST_CALL void onException(const std::string &message);

    MINIKIT_HOST_IMPL virtual
        void onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func);

    MINIKIT_HOST_IMPL virtual
        void onEvaluate(const std::string &name, const std::string &script);

private:
    static MJsVM::ptr sIntance;

private:
    MBaseFunction::ptr mExceptionListener;
};
