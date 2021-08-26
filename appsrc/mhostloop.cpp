#include "mhostloop.h"

struct LaunchItem {
    MLambdaRef listener;
    int priority = 0;
};

struct UpdateItem {
    MLambdaRef listener;
};

m_static_object(sLaunchItems(), std::vector<LaunchItem>)
m_static_object(sUpdateItems(), std::vector<UpdateItem>)

void _MAppLaunch() {
    for (const LaunchItem &item : sLaunchItems()) {
        MLambdaCall(item.listener.get());
    }
}

void _MAppUpdate() {
    for (const UpdateItem &item : sUpdateItems()) {
        MLambdaCall(item.listener.get());
    }
}

void _MAppAddLaunchListener(MLambda *listener, int priority) {
    if (!listener) {
        return;
    }
    
    LaunchItem fresh;
    fresh.listener = m_make_shared listener;
    fresh.priority = priority;
    for (auto it = sLaunchItems().begin(); it != sLaunchItems().end(); ++it) {
        if (priority > (it->priority)) {
            sLaunchItems().insert(it, fresh);
            return;
        }
    }
    sLaunchItems().push_back(fresh);
}

void _MAppAddUpdateListener(MLambda *listener) {
    if (!listener) {
        return;
    }
    
    UpdateItem fresh;
    fresh.listener = m_make_shared listener;
    sUpdateItems().push_back(fresh);
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
