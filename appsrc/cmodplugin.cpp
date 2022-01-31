#include "cmodplugin.h"

CMOD_META(void, IModPlugin, startup , (const cmod_char *name), name);
CMOD_META(void, IModPlugin, shutdown, ());

void CModPlugin::startup(const cmod_char *name) {
}

void CModPlugin::shutdown() {
}
