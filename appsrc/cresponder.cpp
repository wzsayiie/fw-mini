#include "cresponder.h"

CResponder::ptr CResponder::sFocusResponder;

CResponder::ptr CResponder::focusResponder() {
    return sFocusResponder;
}

define_reflectable_class_function(CResponder, becomeFocusResponder)
void CResponder::becomeFocusResponder() {
    if (this == sFocusResponder.get()) {
        return;
    }

    //check.
    if (!canBecomeFocusResponder()) {
        return;
    }
    if (sFocusResponder && !sFocusResponder->canResignFocusResponder()) {
        return;
    }
    
    //transfer:
    CResponder::ptr previous = sFocusResponder;
    sFocusResponder = me();

    if (previous) {
        previous->onResignFocusResponder();
    }
    onBecomeFocusResponder();
}

define_reflectable_class_function(CResponder, resignFocusResponder)
void CResponder::resignFocusResponder() {
    if (this != sFocusResponder.get()) {
        return;
    }

    //check.
    if (!canResignFocusResponder()) {
        return;
    }
    
    //transfer.
    sFocusResponder = nullptr;
    onResignFocusResponder();
}

define_reflectable_class_function(CResponder, isFocusResponder)
bool CResponder::isFocusResponder() {
    return this == sFocusResponder.get();
}

define_reflectable_class_function(CResponder, canBecomeFocusResponder, "virtual")
define_reflectable_class_function(CResponder, canResignFocusResponder, "virtual")

bool CResponder::canBecomeFocusResponder() { implement_injectable_function(bool) return false; }
bool CResponder::canResignFocusResponder() { implement_injectable_function(bool) return true ; }

define_reflectable_class_function(CResponder, findResponder, "virtual;args:evt,pt")
CResponder::ptr CResponder::findResponder(MEvent evt, const MPoint::ptr &pt) {
    implement_injectable_function(CResponder::ptr, evt, pt)
    return nullptr;
}

define_reflectable_class_function(CResponder, existResponder, "virtual;args:responder")
bool CResponder::existResponder(const CResponder::ptr &responder) {
    implement_injectable_function(bool, responder)
    return false;
}

define_reflectable_class_function(CResponder, responseOffset, "virtual")
MPoint::ptr CResponder::responseOffset() {
    implement_injectable_function(MPoint::ptr)
    return MPoint::zero();
}

define_reflectable_class_function(CResponder, canRespond, "args:evt,pt")
bool CResponder::canRespond(MEvent evt, const MPoint::ptr &pt) {
    switch (evt) {
        case MEvent::Touch     : return canRespondTouch     (pt);
        case MEvent::MouseMove : return canRespondMouseMove (pt);
        case MEvent::MouseWheel: return canRespondMouseWheel(pt);
        case MEvent::KbKey     : return canRespondKbKey     ()  ;
        case MEvent::Writing   : return canRespondWriting   ()  ;
        
        default: return false;
    }
}

define_reflectable_class_function(CResponder, canRespondTouch     , "virtual;args:pt")
define_reflectable_class_function(CResponder, canRespondMouseMove , "virtual;args:pt")
define_reflectable_class_function(CResponder, canRespondMouseWheel, "virtual;args:pt")
define_reflectable_class_function(CResponder, canRespondKbKey     , "virtual"        )
define_reflectable_class_function(CResponder, canRespondWriting   , "virtual"        )

bool CResponder::canRespondTouch     (const MPoint::ptr &pt) { implement_injectable_function(bool, pt) return false; }
bool CResponder::canRespondMouseMove (const MPoint::ptr &pt) { implement_injectable_function(bool, pt) return false; }
bool CResponder::canRespondMouseWheel(const MPoint::ptr &pt) { implement_injectable_function(bool, pt) return false; }
bool CResponder::canRespondKbKey     ()                      { implement_injectable_function(bool)     return false; }
bool CResponder::canRespondWriting   ()                      { implement_injectable_function(bool)     return false; }

define_reflectable_class_function(CResponder, onBecomeFocusResponder, "virtual")
define_reflectable_class_function(CResponder, onResignFocusResponder, "virtual")

void CResponder::onBecomeFocusResponder() { implement_injectable_function(void) }
void CResponder::onResignFocusResponder() { implement_injectable_function(void) }

define_reflectable_class_function(CResponder, onTouchBegin, "virtual;args:x,y")
define_reflectable_class_function(CResponder, onTouchMove , "virtual;args:x,y")
define_reflectable_class_function(CResponder, onTouchEnd  , "virtual;args:x,y")
define_reflectable_class_function(CResponder, onMouseEnter, "virtual;args:x,y")
define_reflectable_class_function(CResponder, onMouseMove , "virtual;args:x,y")
define_reflectable_class_function(CResponder, onMouseExit , "virtual;args:x,y")

void CResponder::onTouchBegin(float x, float y) { implement_injectable_function(void, x, y) }
void CResponder::onTouchMove (float x, float y) { implement_injectable_function(void, x, y) }
void CResponder::onTouchEnd  (float x, float y) { implement_injectable_function(void, x, y) }
void CResponder::onMouseEnter(float x, float y) { implement_injectable_function(void, x, y) }
void CResponder::onMouseMove (float x, float y) { implement_injectable_function(void, x, y) }
void CResponder::onMouseExit (float x, float y) { implement_injectable_function(void, x, y) }

define_reflectable_class_function(CResponder, onMouseWheel, "virtual;args:x,y,delta")
void CResponder::onMouseWheel(float x, float y, float delta) {
    implement_injectable_function(void, x, y, delta)
}

define_reflectable_class_function(CResponder, onKbKey, "virtual;args:code")
void CResponder::onKbKey(MKbKeyCode code) {
    implement_injectable_function(void, code)
}

define_reflectable_class_function(CResponder, onWriting, "virtual;args:text")
void CResponder::onWriting(const std::string &text) {
    implement_injectable_function(void, text)
}
