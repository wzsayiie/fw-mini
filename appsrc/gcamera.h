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
    float mFocusX = 0;
    float mFocusY = 0;
};

GCameraObject &GCamera();
