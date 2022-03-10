#include "ccontrol.h"

CControl::CControl(float x, float y, float w, float h): CView(x, y, w, h) {
    //controls are touchable by default.
    setTouchable(true);
}

void CControl::setEventListener(CControlEvent event, const CLambda<void ()> &listener) {
    if (listener) {
        mListeners[event] = listener;
    } else {
        mListeners.erase(event);
    }
}

void CControl::sendEvent(CControlEvent event) {
    auto listener = mListeners.find(event);
    if (listener != mListeners.end()) {
        listener->second();
    }
}

CControlState CControl::state() {
    return mState;
}

void CControl::setState(CControlState state) {
    mState = state;
}
