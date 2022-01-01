#pragma once

#include "clambda.h"
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

c_class(CControl) : public CView {

public:
    CControl(float x, float y, float width, float height);
    
    void setEventListener(CControlEvent event, CLambda<void ()> listener);
    void sendEvent(CControlEvent event);
    
    CControlState state();
    
protected:
    void setState(CControlState state);
    
private:
    std::map<CControlEvent, CLambda<void ()>> _listeners;
    CControlState _state = CControlState::Normal;
};
