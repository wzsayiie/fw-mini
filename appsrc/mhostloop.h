#pragma once

#include "mtypes.h"

const float _MAppUpdateInterval = 0.1f;

MFUNC_HOST void _MAppLaunch();
MFUNC_HOST void _MAppUpdate();

MFUNC_EXPORT void _MAppAddLaunchListener(MLambda *listener, int priority);
MFUNC_EXPORT void _MAppAddUpdateListener(MLambda *listener);

struct _MAppLaunchAdder { _MAppLaunchAdder(void (*func)(), int priority); };
struct _MAppUpdateAdder { _MAppUpdateAdder(void (*func)()); };

const int MAppLaunchPriority_UI    = 2000;
const int MAppLaunchPriority_Scene = 1000;

#define MAPP_LAUNCH(func, priority) ; static _MAppLaunchAdder _launch_##func(func, priority); void func()
#define MAPP_UPDATE(func)           ; static _MAppUpdateAdder _update_##func(func)          ; void func()
