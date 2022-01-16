#include "gdevice.h"

GDeviceObject::GDeviceObject() {
    MLambdaRef lambda = m_cast_lambda [=]() {
        MWindowEvent event = MWindowCurrentEvent();
        
        switch (event) {
            case MWindowEvent_MouseMove: OnMouseMove(); break;
            case MWindowEvent_KeyDown  : OnKeyDown  (); break;
        }
    };
    MWindowAddListener(lambda.get());
}

float GDeviceObject::mouseX() { return MWindowMouseX(); }
float GDeviceObject::mouseY() { return MWindowMouseY(); }

void GDeviceObject::setMouseListener(std::function<void (float x, float y)> listener) {
    _mouseListener = listener;
}

void GDeviceObject::setKeyListener(std::function<void (MKey key)> listener) {
    _keyListener = listener;
}

void GDeviceObject::OnMouseMove() {
    if (_mouseListener) {
        _mouseListener(mouseX(), mouseY());
    }
}

void GDeviceObject::OnKeyDown() {
    if (_keyListener) {
        _keyListener(MWindowActiveKey());
    }
}

def_singleton(GDevice, GDeviceObject);
