#pragma once

#include "boot.h"

def_class(_GCamera) {
    
public:
    void moveFocusTo(float x, float y);
    void setFocusX  (float x);
    void setFocusY  (float y);
    
    float focusX();
    float focusY();
    
private:
    float mFocusX = 0;
    float mFocusY = 0;
};

_GCameraRef &GCamera();
