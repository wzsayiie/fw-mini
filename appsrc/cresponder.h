#pragma once

#include "cobject.h"

class CResponder;
declare_reflectable_special(MFunction<bool (const std::shared_ptr<CResponder> &, const MPoint::ptr &)>)
using CResponderDetector =  MFunction<bool (const std::shared_ptr<CResponder> &, const MPoint::ptr &)>;

c_class(CResponder, CObject) {
public:
    static CResponder::ptr focusResponder();
    
public:
    ~CResponder();
    
public:
    void becomeFocusResponder();
    void resignFocusResponder();
    bool isFocusResponder    ();
    
    virtual CResponder::ptr findResponder(const MPoint::ptr &pt, const CResponderDetector::ptr &fit);
    virtual MPoint::ptr responseOffset();
    
    virtual bool canRespondTouch    (const MPoint::ptr &pt);
    virtual bool canRespondMouseMove(const MPoint::ptr &pt);
    virtual bool canRespondWheel    (const MPoint::ptr &pt);
    virtual bool canRespondKey      (const MPoint::ptr &pt);
    virtual bool canRespondWriting  (const MPoint::ptr &pt);
    
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
    virtual void onWrite     (const std::string &string, bool enter);
    
private:
    static CResponder *sFocusResponder;
};
