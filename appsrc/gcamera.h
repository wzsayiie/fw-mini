#pragma once

#include "boot.h"

def_class(GCameraObject) {
    
public:
    void MoveTo(float x, float y);
    
    void setFocusX(float x);
    void setFocusY(float y);
    
    float focusX();
    float focusY();
    
private:
    float _focusX = 0;
    float _focusY = 0;
};

GCameraObject &GCamera();
