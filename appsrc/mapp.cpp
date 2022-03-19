#include "mapp.h"

define_reflectable_class_const(MApp, UpdateEverySeconds)

define_reflectable_class_function(MApp, sharedObject)
MApp *MApp::sharedObject() {
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

define_reflectable_class_function(MApp, command, "args:line;")
void MApp::command(const std::string &line) {
    for (auto &it : mCommandListeners) {
        it->call(line);
    }
}

define_reflectable_class_function(MApp, addLaunchListener, "args:listener;")
void MApp::addLaunchListener(const MFunction<void ()>::ptr &listener) {
    if (listener) {
        mLaunchListeners.insert(listener);
    }
}

define_reflectable_class_function(MApp, addUpdateListener, "args:listener;")
void MApp::addUpdateListener(const MFunction<void ()>::ptr &listener) {
    if (listener) {
        mUpdateListeners.insert(listener);
    }
}

define_reflectable_class_function(MApp, addCommandListener, "args:listener;")
void MApp::addCommandListener(const MFunction<void (const std::string &)>::ptr &listener) {
    if (listener) {
        mCommandListeners.insert(listener);
    }
}

_MAppLaunchRegistrar::_MAppLaunchRegistrar(void (*fcn)()) {
    auto func = MFunction<void ()>::create(fcn);
    MApp::sharedObject()->addLaunchListener(func);
}
