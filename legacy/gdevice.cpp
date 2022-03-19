#include "gdevice.h"
#include "gcamera.h"

_GDevice::_GDevice() {
    MLambdaRef lambda = m_cast_lambda [=]() {
        MWindowEvent event = MWindowCurrentEvent();
        if (event == MWindowEvent_MouseMove) {
            OnMouseMove();
        }
    };
    MWindowAddListener(lambda.get());
}

float _GDevice::mouseX() {
    return GCamera()->focusX() - MWindowWidth () / 2 + MWindowMouseX();
}

float _GDevice::mouseY() {
    return GCamera()->focusY() + MWindowHeight() / 2 - MWindowMouseY();
}

void _GDevice::setMouseListener(const std::function<void ()> &listener) {
    mMouseListener = listener;
}

void _GDevice::OnMouseMove() {
    if (mMouseListener) {
        mMouseListener();
    }
}

def_singleton(GDevice, _GDeviceRef(new _GDevice))
