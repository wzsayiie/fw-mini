#include "mapp.h"
#include <vector>

struct LaunchItem {
    MLambdaRef listener;
    int priority = 0;
};

struct UpdateItem {
    MLambdaRef listener;
};

static std::vector<LaunchItem> *sLaunchListeners = nullptr;
static std::vector<UpdateItem> *sUpdateListeners = nullptr;

void _MAppLaunch() {
    if (!sLaunchListeners) {
        return;
    }
    for (const LaunchItem &item : *sLaunchListeners) {
        MLambdaCall(item.listener.get());
    }
}

void _MAppUpdate() {
    if (!sUpdateListeners) {
        return;
    }
    for (const UpdateItem &item : *sUpdateListeners) {
        MLambdaCall(item.listener.get());
    }
}

void _MAppAddLaunchListener(MLambda *listener, int priority) {
    if (!listener) {
        return;
    }

    if (!sLaunchListeners) {
        sLaunchListeners = new std::vector<LaunchItem>;
    }

    LaunchItem newItem;
    newItem.listener = m_make_shared listener;
    newItem.priority = priority;

    for (auto it = sLaunchListeners->begin(); it != sLaunchListeners->end(); ++it) {
        if (priority > (it->priority)) {
            sLaunchListeners->insert(it, newItem);
            return;
        }
    }
    sLaunchListeners->push_back(newItem);
}

void _MAppAddUpdateListener(MLambda *listener) {
    if (!listener) {
        return;
    }

    if (!sUpdateListeners) {
        sUpdateListeners = new std::vector<UpdateItem>;
    }
    
    UpdateItem newItem;
    newItem.listener = m_make_shared listener;
    sUpdateListeners->push_back(newItem);
}

_MAppLaunchFuncAdder::_MAppLaunchFuncAdder(void (*func)(), int priority) {
}

_MAppUpdateFuncAdder::_MAppUpdateFuncAdder(void (*func)()) {
}
