#include "gcamera.h"

void GCameraObject::MoveTo(float x, float y) {
    mFocusX = x;
    mFocusY = y;
}

void GCameraObject::setFocusX(float x) { mFocusX = x; }
void GCameraObject::setFocusY(float y) { mFocusY = y; }

float GCameraObject::focusX() { return mFocusX; }
float GCameraObject::focusY() { return mFocusY; }

def_singleton(GCamera, GCameraObject);
