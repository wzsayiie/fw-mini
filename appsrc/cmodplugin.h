#pragma once

#include "cmodule.h"

cmod_intf(IModPlugin, IModObj) {
    
    virtual void startup(const cmod_char *name);
    virtual void shutdown();
};

class CModPlugin : public IModPlugin {
    
public:
    void startup(const cmod_char *name);
    void shutdown();
};
