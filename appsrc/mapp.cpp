#include "mapp.h"

const int FuncSlotNumber = 2;

static void (*sLaunchFuncs[FuncSlotNumber])() = {0};
static void (*sUpdateFuncs[FuncSlotNumber])() = {0};
static int sUpdateFuncCount = 0;

void _MAppLaunch() {
    for (int index = FuncSlotNumber - 1; index >= 0; --index) {
        auto func = sLaunchFuncs[index];
        if (func) {
            func();
        }
    }
}

void _MAppUpdate() {
    for (int index = FuncSlotNumber - 1; index >= 0; --index) {
        auto func = sUpdateFuncs[index];
        if (func) {
            func();
        }
    }
}

void _MAppAddLaunchFunc(void (*func)(), int priority) {
    sLaunchFuncs[priority] = func;
}

void _MAppAddUpdateFunc(void (*func)()) {
    sUpdateFuncs[sUpdateFuncCount++] = func;
}
