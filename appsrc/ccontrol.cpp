#include "ccontrol.h"
#include "dlazy.h"
#include "mcontext.h"
#include "mscheduler.h"
#include "rdefine.h"

static dash::lazy<std::map<std::string, CControl *>> sMap;
static int sIdCount = 0;

CControl::CControl(): CControl(0, 0, 0, 0) {
}

CControl::CControl(float x, float y, float width, float height): super(x, y, width, height) {
    mControlId = std::to_string(++sIdCount);
    sMap->insert({ mControlId, this });
    
    setInteractive(true);
}

CControl::~CControl() {
    sMap->erase(mControlId);
}

define_reflectable_class_function(CControl, setControlId, "setter;args:iden")
void CControl::setControlId(const std::string &iden) {
    //own control id should not be empty.
    if (iden.empty()) {
        return;
    }
    
    sMap->erase(mControlId);
    sMap->insert({ iden, this });
    mControlId = iden;
}

define_reflectable_class_function(CControl, controlId, "getter")
std::string CControl::controlId() {
    return mControlId;
}

define_reflectable_class_function(CControl, setPreviousControl, "setter;args:iden")
void CControl::setPreviousControl(const std::string &iden) {
    mPreviousControl = iden;
}

define_reflectable_class_function(CControl, setNextControl, "setter;args:iden")
void CControl::setNextControl(const std::string &iden) {
    mNextControl = iden;
}

define_reflectable_class_function(CControl, previousControl, "getter")
std::string CControl::previousControl() {
    return mPreviousControl;
}

define_reflectable_class_function(CControl, nextControl, "getter")
std::string CControl::nextControl() {
    return mNextControl;
}

define_reflectable_class_function(CControl, transferFocusToPrevious)
void CControl::transferFocusToPrevious() {
    CControl::ptr previous = checkoutControl(mPreviousControl);
    if (previous && previous.get() != this) {
        previous->becomeFocusResponder();
    }
}

define_reflectable_class_function(CControl, transferFocusToNext)
void CControl::transferFocusToNext() {
    CControl::ptr next = checkoutControl(mNextControl);
    if (next && next.get() != this) {
        next->becomeFocusResponder();
    }
}

define_reflectable_class_function(CControl, transferFocusToAny)
void CControl::transferFocusToAny() {
    //try transferring to next.
    CControl::ptr next = checkoutControl(mNextControl);
    if (next && next.get() != this) {
        next->becomeFocusResponder();
        
        if (next->isFocusResponder()) {
            return;
        }
    }

    //try transferring to previous.
    CControl::ptr previous = checkoutControl(mPreviousControl);
    if (previous && previous.get() != this) {
        previous->becomeFocusResponder();
        
        if (previous->isFocusResponder()) {
            return;
        }
    }
    
    //discard focus.
    resignFocusResponder();
}

void CControl::onDrawCover(float w, float h) {
    if (!isFocusResponder()) {
        return;
    }
    
    //draw the blinking cursor:
    int64_t n = (int64_t)MScheduler::instance()->secondsTick();
    if (n % 2 == 0) {
        return;
    }
    
    MContextSelectRGBA(MColor::BlackRGBA);
    const float t = 10.f;
    
    //upper left.
    MContextMoveToPoint(0, 0);
    MContextAddToPoint (t, 0);
    MContextAddToPoint (0, t);
    MContextDrawPolygon();
    //upper right.
    MContextMoveToPoint(w  , 0);
    MContextAddToPoint (w  , t);
    MContextAddToPoint (w-t, 0);
    MContextDrawPolygon();
    //lower right.
    MContextMoveToPoint(w  , h  );
    MContextAddToPoint (w-t, h  );
    MContextAddToPoint (w  , h-t);
    MContextDrawPolygon();
    //lower left.
    MContextMoveToPoint(0, h  );
    MContextAddToPoint (0, h-t);
    MContextAddToPoint (t, h  );
    MContextDrawPolygon();
}

void CControl::onKbKey(MKbKeyCode code) {
    if (code == MKbKeyCode::Tab) {
        MKbKey::ptr key = MCurrentKbKey();

        if (key->modifiers() & MKbModifier_Shift) {
            transferFocusToPrevious();
        } else {
            transferFocusToAny();
        }

    } else {
        //pass the event to subclass.
        onControlKbKey(code);
    }
}

define_reflectable_class_function(CControl, onControlKbKey, "virtual;args:code")
void CControl::onControlKbKey(MKbKeyCode code) {
    implement_injectable_function(void, code)
}

CControl::ptr CControl::checkoutControl(const std::string &iden) {
    if (iden.empty()) {
        return nullptr;
    }

    auto target = sMap->find(iden);
    if (target == sMap->end()) {
        return nullptr;
    }
    
    return target->second->me();
}
