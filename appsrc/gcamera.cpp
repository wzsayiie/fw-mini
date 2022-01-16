#include "gcamera.h"

void GCameraObject::MoveTo(float x, float y) {
    _focusX = x;
    _focusY = y;
}

void GCameraObject::setFocusX(float x) { _focusX = x; }
void GCameraObject::setFocusY(float y) { _focusY = y; }

float GCameraObject::focusX() { return _focusX; }
float GCameraObject::focusY() { return _focusY; }

def_singleton(GCamera, GCameraObject);
