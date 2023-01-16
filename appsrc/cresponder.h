#pragma once

#include "cobject.h"
#include "mevent.h"
#include "mgeometry.h"

m_class(CResponder, CObject) {
public:
    static CResponder::ptr focusResponder();
    
public:
    void becomeFocusResponder();
    void resignFocusResponder();
    bool isFocusResponder    ();

    virtual bool canBecomeFocusResponder();
    virtual bool canResignFocusResponder();
    
    virtual CResponder::ptr findResponder (MEvent evt, const MPoint::ptr &pt);
    virtual bool            existResponder(const CResponder::ptr &responder);
    virtual MPoint::ptr     responseOffset();
    
    bool canRespond(MEvent evt, const MPoint::ptr &pt);
    
    virtual bool canRespondTouch     (const MPoint::ptr &pt);
    virtual bool canRespondMouseMove (const MPoint::ptr &pt);
    virtual bool canRespondMouseWheel(const MPoint::ptr &pt);
    virtual bool canRespondKbKey     ();
    virtual bool canRespondWriting   ();
    
protected: public:
    virtual void onBecomeFocusResponder();
    virtual void onResignFocusResponder();

    virtual void onTouchBegin(float x, float y);
    virtual void onTouchMove (float x, float y);
    virtual void onTouchEnd  (float x, float y);
    virtual void onMouseEnter(float x, float y);
    virtual void onMouseMove (float x, float y);
    virtual void onMouseExit (float x, float y);
    virtual void onMouseWheel(float x, float y, float delta);
    virtual void onKbKey     (MKbKeyCode code);
    virtual void onWriting   (const std::string &text);
    
private:
    static CResponder::ptr sFocusResponder;
};
