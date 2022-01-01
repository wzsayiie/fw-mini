#include "ccontrol.h"

CControl::CControl(float x, float y, float w, float h): CView(x, y, w, h) {
    //controls are touchable by default.
    setTouchable(true);
}

void CControl::setEventListener(CControlEvent event, CLambda<void ()> listener) {
    if (listener) {
        _listeners[event] = listener;
    } else {
        _listeners.erase(event);
    }
}

void CControl::sendEvent(CControlEvent event) {
    auto listener = _listeners.find(event);
    if (listener != _listeners.end()) {
        listener->second();
    }
}

CControlState CControl::state() {
    return _state;
}

void CControl::setState(CControlState state) {
    _state = state;
}
