#include "mapp.h"
#include "rdefine.h"

//app:

define_reflectable_class_const(MApp, UpdateEverySeconds)

define_reflectable_class_function(MApp, instance, "getter")
MApp *MApp::instance() {
    static auto obj = MApp::create();
    return obj.get();
}

define_reflectable_class_function(MApp, launch)
void MApp::launch() {
    for (auto &it : mLaunchListeners) {
        it.listener->call();
    }
}

define_reflectable_class_function(MApp, update)
void MApp::update() {
    for (auto &it : mUpdateListeners) {
        it.listener->call();
    }
}

define_reflectable_class_function(MApp, addLaunchListener, "args:priority,listener")
void MApp::addLaunchListener(int priority, const MFunction<void ()>::ptr &listener) {
    addListener(&mLaunchListeners, priority, listener);
}

define_reflectable_class_function(MApp, addUpdateListener, "args:priority,listener")
void MApp::addUpdateListener(int priority, const MFunction<void ()>::ptr &listener) {
    addListener(&mUpdateListeners, priority, listener);
}

define_reflectable_class_function(MApp, removeLaunchListener, "args:listener")
void MApp::removeLaunchListener(const MFunction<void ()>::ptr &listener) {
    removeListener(&mLaunchListeners, listener);
}

define_reflectable_class_function(MApp, removeUpdateListener, "args:listener")
void MApp::removeUpdateListener(const MFunction<void ()>::ptr &listener) {
    removeListener(&mUpdateListeners, listener);
}

void MApp::addListener(
    std::vector<ListenerItem> *items, int priority, const MFunction<void ()>::ptr &listener)
{
    if (!listener) {
        return;
    }

    auto pos = items->begin();
    for (; pos != items->end(); ++pos) {
        if (pos->priority > priority) {
            break;
        }
    }

    ListenerItem item;
    item.listener = listener;
    item.priority = priority;
    items->insert(pos, item);
}

void MApp::removeListener(
    std::vector<ListenerItem> *items, const MFunction<void ()>::ptr &listener)
{
    if (!listener) {
        return;
    }

    items->erase(
        std::remove_if(items->begin(), items->end(), [=](const ListenerItem &item) {
            return item.listener == listener;
        }),
        items->end()
    );
}

//launch registrar:

_MAppLaunchRegistrar::_MAppLaunchRegistrar(void (*fcn)(), int priority) noexcept {
    MApp::instance()->addLaunchListener(priority, MF(fcn));
}
