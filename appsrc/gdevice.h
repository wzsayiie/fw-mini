#pragma once

#include "minikit.h"

def_class(_GDevice) {
    
public:
    _GDevice();
    
    float mouseX();
    float mouseY();
    
    void setMouseListener(std::function<void (float x, float y)> listener);
    void setKeyListener(std::function<void (MKey key)> listener);
    
private:
    void OnMouseMove();
    void OnKeyDown();
    
    std::function<void (float x, float y)> mMouseListener;
    std::function<void (MKey key)> mKeyListener;
};

_GDeviceRef &GDevice();
