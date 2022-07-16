#pragma once

#include "cobject.h"

//response event:

m_enum(CResponseEvent) {
    Touch     = 1,
    MouseMove = 2,
    Wheel     = 3,
    Key       = 4,
    Writing   = 5,
};

//responder:

m_class(CResponder, CObject) {
public:
    static CResponder::ptr focusResponder();
    
public:
    void becomeFocusResponder();
    void resignFocusResponder();
    bool isFocusResponder    ();

    virtual bool canBecomeFocusResponder();
    virtual bool canResignFocusResponder();
    
    virtual CResponder::ptr findResponder (CResponseEvent event, const MPoint::ptr &pt);
    virtual bool            existResponder(const CResponder::ptr &responder);
    virtual MPoint::ptr     responseOffset();
    
    bool canRespond(CResponseEvent event, const MPoint::ptr &pt);
    
    virtual bool canRespondTouch    (const MPoint::ptr &pt);
    virtual bool canRespondMouseMove(const MPoint::ptr &pt);
    virtual bool canRespondWheel    (const MPoint::ptr &pt);
    virtual bool canRespondKey      ();
    virtual bool canRespondWriting  ();
    
protected: public:
    virtual void onBecomeFocusResponder();
    virtual void onResignFocusResponder();

    virtual void onTouchBegin(float x, float y);
    virtual void onTouchMove (float x, float y);
    virtual void onTouchEnd  (float x, float y);
    virtual void onMouseEnter(float x, float y);
    virtual void onMouseMove (float x, float y);
    virtual void onMouseExit (float x, float y);
    virtual void onWheel     (float delta);
    virtual void onKey       (MKey key);
    virtual void onWrite     (const std::string &text, MKey key);
    
private:
    static CResponder::ptr sFocusResponder;
};
