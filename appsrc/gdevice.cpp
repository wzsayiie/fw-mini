#include "gdevice.h"
#include "gcamera.h"

_GDevice::_GDevice() {
    MLambdaRef lambda = m_cast_lambda [=]() {
        MWindowEvent event = MWindowCurrentEvent();
        
        switch (event) {
            case MWindowEvent_MouseMove: OnMouseMove(); break;
            case MWindowEvent_KeyDown  : OnKeyDown  (); break;
        }
    };
    MWindowAddListener(lambda.get());
}

float _GDevice::mouseX() { return GCamera()->focusX() + MWindowWidth () / 2 - MWindowMouseX(); }
float _GDevice::mouseY() { return GCamera()->focusY() + MWindowHeight() / 2 - MWindowMouseY(); }

void _GDevice::setMouseListener(std::function<void (float x, float y)> listener) {
    mMouseListener = listener;
}

void _GDevice::setKeyListener(std::function<void (MKey key)> listener) {
    mKeyListener = listener;
}

void _GDevice::OnMouseMove() {
    if (mMouseListener) {
        mMouseListener(mouseX(), mouseY());
    }
}

void _GDevice::OnKeyDown() {
    if (mKeyListener) {
        mKeyListener(MWindowActiveKey());
    }
}

def_singleton(GDevice, _GDeviceRef(new _GDevice));
