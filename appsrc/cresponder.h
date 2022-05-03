#pragma once

#include "cobject.h"

class CResponder;
declare_reflectable_special(MFunction<bool (const std::shared_ptr<CResponder> &, float, float)>)
using CResponderDetector =  MFunction<bool (const std::shared_ptr<CResponder> &, float, float)>;

c_class(CResponder, CObject) {
public:
    static CResponder::ptr focusResponder();
    
public:
    ~CResponder();
    
public:
    void becomeFocusResponder();
    void resignFocusResponder();
    bool isFocusResponder    ();
    
    virtual CResponder::ptr findResponder(float x, float y, const CResponderDetector::ptr &fit);
    virtual MPoint::ptr responseOffset();
    
    virtual bool canRespondTouch    (float x, float y);
    virtual bool canRespondMouseMove(float x, float y);
    virtual bool canRespondWriting  (float x, float y);
    virtual bool canRespondKey      (float x, float y);
    virtual bool canRespondWheel    (float x, float y);
    
protected: public:
    virtual void onBecomeFocusResponder();
    virtual void onResignFocusResponder();

    virtual void onTouchBegin(float x, float y);
    virtual void onTouchMove (float x, float y);
    virtual void onTouchEnd  (float x, float y);
    virtual void onMouseEnter(float x, float y);
    virtual void onMouseMove (float x, float y);
    virtual void onMouseExit (float x, float y);
    virtual void onWrite     (const std::string &string, bool enter);
    virtual void onKey       (MKey key);
    virtual void onWheel     (float delta);
    
private:
    static CResponder *sFocusResponder;
};
