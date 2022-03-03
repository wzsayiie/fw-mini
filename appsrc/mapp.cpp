#include "mapp.h"

define_reflectable_const(MAppUpdateIntervalSeconds)

define_reflectable_enum_const(MAppEvent, Launch )
define_reflectable_enum_const(MAppEvent, Update )
define_reflectable_enum_const(MAppEvent, Command)

define_reflectable_class_function(MApp, shared)
MApp *MApp::shared() {
    static auto object = new MApp();
    return object;
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

define_reflectable_class_function(MApp, command)
void MApp::command(const std::string &line) {
    mCommand = line;

    for (auto &it : mCommandListeners) {
        it->call();
    }

    mCommand.clear();
}

define_reflectable_class_function(MApp, addListener)
void MApp::addListener(MAppEvent event, const MFunction<void ()>::ptr &listener) {
    if (!listener) {
        return;
    }

    if /**/ (event == MAppEvent::Launch ) { mLaunchListeners .insert(listener); }
    else if (event == MAppEvent::Update ) { mUpdateListeners .insert(listener); }
    else if (event == MAppEvent::Command) { mCommandListeners.insert(listener); }
}

define_reflectable_class_function(MApp, removeListener)
void MApp::removeListener(MAppEvent event, const MFunction<void ()>::ptr &listener) {
    if (!listener) {
        return;
    }

    if /**/ (event == MAppEvent::Launch ) { mLaunchListeners .erase(listener); }
    else if (event == MAppEvent::Update ) { mUpdateListeners .erase(listener); }
    else if (event == MAppEvent::Command) { mCommandListeners.erase(listener); }
}

define_reflectable_class_function(MApp, currentCommand)
const std::string &MApp::currentCommand() {
    return mCommand;
}

MAppLaunchRegister::MAppLaunchRegister(void (*fcn)()) {
    auto func = MFunction<void ()>::create(fcn);
    MApp::shared()->addListener(MAppEvent::Launch, func);
}
