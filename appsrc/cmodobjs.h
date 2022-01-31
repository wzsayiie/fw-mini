#pragma once

#include "cmodmeta.h"

cmod_intf(IPlugin, IModObj) {
    
    virtual void startup(const cmod_char *name);
    virtual void shutdown();
};

class CPlugin : public IPlugin {
    
public:
    void startup(const cmod_char *name);
    void shutdown();
};
