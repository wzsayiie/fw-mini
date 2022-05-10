#include "cresponder.h"

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
    
    onResignFocusResponder();
    sFocusResponder = nullptr;
}

define_reflectable_class_function(CResponder, isFocusResponder)
bool CResponder::isFocusResponder() {
    return this == sFocusResponder;
}

define_reflectable_class_function(CResponder, findResponder, "args:pt,fit")
CResponder::ptr CResponder::findResponder(const MPoint::ptr &pt, const CResponderDetector::ptr &fit) {
    implement_injectable_function((CResponder::ptr), pt, fit)
    return nullptr;
}

define_reflectable_class_function(CResponder, responseOffset)
MPoint::ptr CResponder::responseOffset() {
    implement_injectable_function((MPoint::ptr))
    return MPoint::zero();
}

define_reflectable_class_function(CResponder, canRespondTouch    , "args:pt")
define_reflectable_class_function(CResponder, canRespondMouseMove, "args:pt")
define_reflectable_class_function(CResponder, canRespondWheel    , "args:pt")
define_reflectable_class_function(CResponder, canRespondKey      , "args:pt")
define_reflectable_class_function(CResponder, canRespondWriting  , "args:pt")

bool CResponder::canRespondTouch    (const MPoint::ptr &pt) { implement_injectable_function((bool), pt) return false; }
bool CResponder::canRespondMouseMove(const MPoint::ptr &pt) { implement_injectable_function((bool), pt) return false; }
bool CResponder::canRespondWheel    (const MPoint::ptr &pt) { implement_injectable_function((bool), pt) return false; }
bool CResponder::canRespondKey      (const MPoint::ptr &pt) { implement_injectable_function((bool), pt) return false; }
bool CResponder::canRespondWriting  (const MPoint::ptr &pt) { implement_injectable_function((bool), pt) return false; }

define_reflectable_class_function(CResponder, onBecomeFocusResponder)
define_reflectable_class_function(CResponder, onResignFocusResponder)

void CResponder::onBecomeFocusResponder() { implement_injectable_function((void)) }
void CResponder::onResignFocusResponder() { implement_injectable_function((void)) }

define_reflectable_class_function(CResponder, onTouchBegin, "args:x,y")
define_reflectable_class_function(CResponder, onTouchMove , "args:x,y")
define_reflectable_class_function(CResponder, onTouchEnd  , "args:x,y")
define_reflectable_class_function(CResponder, onMouseEnter, "args:x,y")
define_reflectable_class_function(CResponder, onMouseMove , "args:x,y")
define_reflectable_class_function(CResponder, onMouseExit , "args:x,y")

void CResponder::onTouchBegin(float x, float y) { implement_injectable_function((void), x, y) }
void CResponder::onTouchMove (float x, float y) { implement_injectable_function((void), x, y) }
void CResponder::onTouchEnd  (float x, float y) { implement_injectable_function((void), x, y) }
void CResponder::onMouseEnter(float x, float y) { implement_injectable_function((void), x, y) }
void CResponder::onMouseMove (float x, float y) { implement_injectable_function((void), x, y) }
void CResponder::onMouseExit (float x, float y) { implement_injectable_function((void), x, y) }

define_reflectable_class_function(CResponder, onWheel, "args:delta")
void CResponder::onWheel(float delta) {
    implement_injectable_function((void), delta)
}

define_reflectable_class_function(CResponder, onKey, "args:key")
void CResponder::onKey(MKey key) {
    implement_injectable_function((void), (int)key)
}

define_reflectable_class_function(CResponder, onWrite, "args:string,enter")
void CResponder::onWrite(const std::string &string, bool enter) {
    implement_injectable_function((void), string, enter)
}
