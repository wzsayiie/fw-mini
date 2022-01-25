#pragma once

#include "minikit.h"

def_class(_GDevice) {
    
public:
    _GDevice();
    
    float mouseX();
    float mouseY();
    
    void setMouseListener(std::function<void ()> listener);
    
private:
    void OnMouseMove();
    
    std::function<void ()> mMouseListener;
};

_GDeviceRef &GDevice();
