#include "cresponder.h"

//response event:

define_reflectable_enum_const(CResponseEvent, Touch    )
define_reflectable_enum_const(CResponseEvent, MouseMove)
define_reflectable_enum_const(CResponseEvent, Wheel    )
define_reflectable_enum_const(CResponseEvent, Key      )
define_reflectable_enum_const(CResponseEvent, Writing  )

//responder:

CResponder *CResponder::sFocusResponder = nullptr;

CResponder::ptr CResponder::focusResponder() {
    if (sFocusResponder) {
        return sFocusResponder->shared();
    }
    return nullptr;
}

CResponder::~CResponder() {
    if (this == sFocusResponder) {
        sFocusResponder = nullptr;
    }
}

define_reflectable_class_function(CResponder, becomeFocusResponder)
void CResponder::becomeFocusResponder() {
    if (this == sFocusResponder) {
        return;
    }

    //check.
    if (!canBecomeFocusResponder()) {
        return;
    }
    if (sFocusResponder && !sFocusResponder->canResignFocusResponder()) {
        return;
    }
    
    //transfer.
    if (sFocusResponder) {
        sFocusResponder->onResignFocusResponder();
    }
    onBecomeFocusResponder();
    sFocusResponder = this;
}

define_reflectable_class_function(CResponder, resignFocusResponder)
void CResponder::resignFocusResponder() {
    if (this != sFocusResponder) {
        return;
    }

    //check.
    if (!canResignFocusResponder()) {
        return;
    }
    
    //transfer.
    onResignFocusResponder();
    sFocusResponder = nullptr;
}

define_reflectable_class_function(CResponder, isFocusResponder)
bool CResponder::isFocusResponder() {
    return this == sFocusResponder;
}

define_reflectable_class_function(CResponder, canBecomeFocusResponder, "virtual")
define_reflectable_class_function(CResponder, canResignFocusResponder, "virtual")

bool CResponder::canBecomeFocusResponder() { implement_injectable_function(bool) return false; }
bool CResponder::canResignFocusResponder() { implement_injectable_function(bool) return true ; }

define_reflectable_class_function(CResponder, findResponder, "virtual;args:event,pt")
CResponder::ptr CResponder::findResponder(CResponseEvent event, const MPoint::ptr &pt) {
    implement_injectable_function(CResponder::ptr, event, pt)
    return nullptr;
}

define_reflectable_class_function(CResponder, responseOffset, "virtual")
MPoint::ptr CResponder::responseOffset() {
    implement_injectable_function(MPoint::ptr)
    return MPoint::zero();
}

define_reflectable_class_function(CResponder, canRespond, "args:event,pt")
bool CResponder::canRespond(CResponseEvent event, const MPoint::ptr &pt) {
    switch (event) {
        case CResponseEvent::Touch    : return canRespondTouch    (pt);
        case CResponseEvent::MouseMove: return canRespondMouseMove(pt);
        case CResponseEvent::Wheel    : return canRespondWheel    (pt);
        case CResponseEvent::Key      : return canRespondKey      ()  ;
        case CResponseEvent::Writing  : return canRespondWriting  ()  ;
        
        default: return false;
    }
}

define_reflectable_class_function(CResponder, canRespondTouch    , "virtual;args:pt")
define_reflectable_class_function(CResponder, canRespondMouseMove, "virtual;args:pt")
define_reflectable_class_function(CResponder, canRespondWheel    , "virtual;args:pt")
define_reflectable_class_function(CResponder, canRespondKey      , "virtual"        )
define_reflectable_class_function(CResponder, canRespondWriting  , "virtual"        )

bool CResponder::canRespondTouch    (const MPoint::ptr &pt) { implement_injectable_function(bool, pt) return false; }
bool CResponder::canRespondMouseMove(const MPoint::ptr &pt) { implement_injectable_function(bool, pt) return false; }
bool CResponder::canRespondWheel    (const MPoint::ptr &pt) { implement_injectable_function(bool, pt) return false; }
bool CResponder::canRespondKey      ()                      { implement_injectable_function(bool)     return false; }
bool CResponder::canRespondWriting  ()                      { implement_injectable_function(bool)     return false; }

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

define_reflectable_class_function(CResponder, onWheel, "virtual;args:delta")
void CResponder::onWheel(float delta) {
    implement_injectable_function(void, delta)
}

define_reflectable_class_function(CResponder, onKey, "virtual;args:key")
void CResponder::onKey(MKey key) {
    implement_injectable_function(void, key)
}

define_reflectable_class_function(CResponder, onWrite, "virtual;args:text,key")
void CResponder::onWrite(const std::string &text, MKey key) {
    implement_injectable_function(void, text, key)
}
