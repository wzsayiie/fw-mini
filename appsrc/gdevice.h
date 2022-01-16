#pragma once

#include "minikit.h"

def_class(GDeviceObject) {
    
public:
    GDeviceObject();
    
    float mouseX();
    float mouseY();
    
    void setMouseListener(std::function<void (float x, float y)> listener);
    void setKeyListener(std::function<void (MKey key)> listener);
    
private:
    void OnMouseMove();
    void OnKeyDown();
    
    std::function<void (float x, float y)> _mouseListener;
    std::function<void (MKey key)> _keyListener;
};

GDeviceObject &GDevice();
