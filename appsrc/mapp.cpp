#include "mapp.h"

//app:

define_reflectable_class_const(MApp, UpdateEverySeconds)

define_reflectable_class_function(MApp, instance)
MApp *MApp::instance() {
    static auto obj = MApp::create();
    return obj.get();
}

define_reflectable_class_function(MApp, launch)
void MApp::launch() {
    for (auto &it : mLaunchListeners) {
        it->call();
    }
}

define_reflectable_class_function(MApp, update)
void MApp::update() {
    for (auto &it : mUpdateListeners) {
        it->call();
    }
}

define_reflectable_class_function(MApp, addLaunchListener, "args:listener")
void MApp::addLaunchListener(const MFunction<void ()>::ptr &listener) {
    if (listener) {
        mLaunchListeners.insert(listener);
    }
}

define_reflectable_class_function(MApp, addUpdateListener, "args:listener")
void MApp::addUpdateListener(const MFunction<void ()>::ptr &listener) {
    if (listener) {
        mUpdateListeners.insert(listener);
    }
}

define_reflectable_class_function(MApp, removeListener, "args:listener")
void MApp::removeListener(const MFunction<void ()>::ptr &listener) {
    if (listener) {
        mLaunchListeners.erase(listener);
        mUpdateListeners.erase(listener);
    }
}

//launch registrar:

_MAppLaunchRegistrar::_MAppLaunchRegistrar(void (*fcn)()) noexcept {
    auto func = MFunction<void ()>::create(fcn);
    MApp::instance()->addLaunchListener(func);
}
