#pragma once

#include "mconfig.h"

const float _MAppUpdateInterval = 0.1f;

MFUNC_HOST void _MAppLaunch();
MFUNC_HOST void _MAppUpdate();

MFUNC_BASE void _MAppAddLaunchFunc(void (*func)(), int priority);
MFUNC_BASE void _MAppAddUpdateFunc(void (*func)());

struct _MAppLaunchAdder {
    _MAppLaunchAdder(void (*func)(), int priority) {
        _MAppAddLaunchFunc(func, priority);
    }
};

struct _MAppUpdateAdder {
    _MAppUpdateAdder(void (*func)()) {
        _MAppAddUpdateFunc(func);
    }
};

#define MAPP_SCENE_LAUNCH(N) void N(); static _MAppLaunchAdder _launch_##N(N, 1);
#define MAPP_UI_LAUNCH(   N) void N(); static _MAppLaunchAdder _launch_##N(N, 0);
#define MAPP_UPDATE(      N) void N(); static _MAppUpdateAdder _update_##N(N);
