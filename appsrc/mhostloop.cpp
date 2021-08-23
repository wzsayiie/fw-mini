#include "mhostloop.h"

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
    
    LaunchItem fresh;
    fresh.listener = m_make_shared listener;
    fresh.priority = priority;
    for (auto it = sLaunchItems->begin(); it != sLaunchItems->end(); ++it) {
        if (priority > (it->priority)) {
            sLaunchItems->insert(it, fresh);
            return;
        }
    }
    sLaunchItems->push_back(fresh);
}

void _MAppAddUpdateListener(MLambda *listener) {
    if (!listener) {
        return;
    }
    
    if (!sUpdateItems) {
        sUpdateItems = new std::vector<UpdateItem>;
    }
    
    UpdateItem fresh;
    fresh.listener = m_make_shared listener;
    sUpdateItems->push_back(fresh);
}

_MAppLaunchAdder::_MAppLaunchAdder(void (*func)(), int priority) {
    MLambdaRef listener = m_cast_lambda [func]() {
        func();
    };
    _MAppAddLaunchListener(listener.get(), priority);
}

_MAppUpdateAdder::_MAppUpdateAdder(void (*func)()) {
    MLambdaRef listener = m_cast_lambda [func]() {
        func();
    };
    _MAppAddUpdateListener(listener.get());
}
