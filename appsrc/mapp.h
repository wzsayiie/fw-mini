#pragma once

#include "mtypes.h"

const float _MAppUpdateInterval = 0.1f;

MFUNC_HOST void _MAppLaunch();
MFUNC_HOST void _MAppUpdate();

MFUNC_EXPORT void _MAppAddLaunchListener(MLambda *listener, int priority);
MFUNC_EXPORT void _MAppAddUpdateListener(MLambda *listener);

struct _MAppLaunchFuncAdder { _MAppLaunchFuncAdder(void (*func)(), int priority); };
struct _MAppUpdateFuncAdder { _MAppUpdateFuncAdder(void (*func)()); };

#define MAPP_SCENE_LAUNCH(N) ; static _MAppLaunchFuncAdder _launch_##N(N, 1); void N()
#define MAPP_UI_LAUNCH(   N) ; static _MAppLaunchFuncAdder _launch_##N(N, 0); void N()
#define MAPP_UPDATE(      N) ; static _MAppUpdateFuncAdder _update_##N(N)   ; void N()
