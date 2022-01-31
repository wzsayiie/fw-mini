#include "cmodobjs.h"

CMOD_META(void, IPlugin, startup , (const cmod_char *name), name);
CMOD_META(void, IPlugin, shutdown, ());

void CPlugin::startup(const cmod_char *name) {
}

void CPlugin::shutdown() {
}
