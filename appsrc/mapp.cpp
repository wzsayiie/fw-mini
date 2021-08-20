#include "mapp.h"

struct LaunchItem {
    MLambdaRef listener;
    int priority = 0;
};

struct UpdateItem {
    MLambdaRef listener;
};

static std::vector<LaunchItem> *sLaunchItems = nullptr;
static std::vector<UpdateItem> *sUpdateItems = nullptr;

void _MAppLaunch() {
    if (!sLaunchItems) {
        return;
    }
    for (const LaunchItem &item : *sLaunchItems) {
        MLambdaCall(item.listener.get());
    }
}

void _MAppUpdate() {
    if (!sUpdateItems) {
        return;
    }
    for (const UpdateItem &item : *sUpdateItems) {
        MLambdaCall(item.listener.get());
    }
}

void _MAppAddLaunchListener(MLambda *listener, int priority) {
    if (!listener) {
        return;
    }
    
    if (!sLaunchItems) {
        sLaunchItems = new std::vector<LaunchItem>;
    }
    
    LaunchItem one;
    one.listener = m_make_shared listener;
    one.priority = priority;
    for (auto it = sLaunchItems->begin(); it != sLaunchItems->end(); ++it) {
        if (priority > (it->priority)) {
            sLaunchItems->insert(it, one);
            return;
        }
    }
    sLaunchItems->push_back(one);
}

void _MAppAddUpdateListener(MLambda *listener) {
    if (!listener) {
        return;
    }
    
    if (!sUpdateItems) {
        sUpdateItems = new std::vector<UpdateItem>;
    }
    
    UpdateItem one;
    one.listener = m_make_shared listener;
    sUpdateItems->push_back(one);
}

_MAppLaunchFuncAdder::_MAppLaunchFuncAdder(void (*func)(), int priority) {
    MLambdaRef listener = m_cast_lambda [func]() {
        func();
    };
    _MAppAddLaunchListener(listener.get(), priority);
}

_MAppUpdateFuncAdder::_MAppUpdateFuncAdder(void (*func)()) {
    MLambdaRef listener = m_cast_lambda [func]() {
        func();
    };
    _MAppAddUpdateListener(listener.get());
}
