#include "gcamera.h"

void _GCamera::moveFocusTo(float x, float y) {
    mFocusX = x;
    mFocusY = y;
}

void _GCamera::setFocusX(float x) { mFocusX = x; }
void _GCamera::setFocusY(float y) { mFocusY = y; }

float _GCamera::focusX() { return mFocusX; }
float _GCamera::focusY() { return mFocusY; }

def_singleton(GCamera, _GCameraRef(new _GCamera));
