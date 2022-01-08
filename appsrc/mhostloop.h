#pragma once

#include "mbasics.h"

const float _MAppUpdateInterval = 0.1f;

M_FUNC_HOST void _MAppLaunch();
M_FUNC_HOST void _MAppUpdate();

M_FUNC_EXPORT void MAppAddLaunchListener(MLambda *listener, int priority) M_META(MAppAddLaunchListener, "args:listener,priority");
M_FUNC_EXPORT void MAppAddUpdateListener(MLambda *listener)               M_META(MAppAddUpdateListener, "args:listener");

struct _MAppLaunchAdder { _MAppLaunchAdder(void (*func)(), int priority); };
struct _MAppUpdateAdder { _MAppUpdateAdder(void (*func)()); };

const int MAppLaunchPriority_Scene = 2000;
const int MAppLaunchPriority_UI    = 1000;

#define MAPP_LAUNCH(func, priority) ; static _MAppLaunchAdder _launch_##func(func, priority); void func()
#define MAPP_UPDATE(func)           ; static _MAppUpdateAdder _update_##func(func)          ; void func()
