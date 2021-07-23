#pragma once

#include "mtypes.h"

const float _MAppUpdateInterval = 0.1f;

MFUNC_HOST void _MAppLaunch();
MFUNC_HOST void _MAppUpdate();

MFUNC_BASE void _MAppAddLaunchListener(MLambda *listener, int priority);
MFUNC_BASE void _MAppAddUpdateListener(MLambda *listener);

struct _MAppLaunchFuncAdder { _MAppLaunchFuncAdder(void (*func)(), int priority); };
struct _MAppUpdateFuncAdder { _MAppUpdateFuncAdder(void (*func)()); };

#define MAPP_SCENE_LAUNCH(N) void N(); static _MAppLaunchFuncAdder _launch_##N(N, 1);
#define MAPP_UI_LAUNCH(   N) void N(); static _MAppLaunchFuncAdder _launch_##N(N, 0);
#define MAPP_UPDATE(      N) void N(); static _MAppUpdateFuncAdder _update_##N(N);
