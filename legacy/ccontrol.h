#pragma once

#include "cview.h"

enum class CControlState {
    Normal   = 0,
    Selected = 1,
};

enum class CControlEvent {
    TouchDown      = 0,
    TouchUpInside  = 1,
    TouchUpOutside = 2,
};

def_class(CControl) : public CView {

public:
    CControl(float x, float y, float width, float height);
    
    void setEventListener(CControlEvent event, const CLambda<void ()> &listener);
    void sendEvent(CControlEvent event);
    
    CControlState state();
    
protected:
    void setState(CControlState state);
    
private:
    std::map<CControlEvent, CLambda<void ()>> mListeners;
    CControlState mState = CControlState::Normal;
};
