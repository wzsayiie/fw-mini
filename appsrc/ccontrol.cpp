#include "ccontrol.h"

static dash::lazy<std::map<std::string, CControl *>> sMap;
static dash::lazy<std::set<CControl *>> sSet;
static int sIdCount = 0;

CControl::CControl() {
    mControlId = std::to_string(++sIdCount);
    sMap->insert({ mControlId, this });
    sSet->insert(this);
    
    setInteractive(true);
}

CControl::CControl(float x, float y, float width, float height): CControl() {
    setFrame(MRect::from(x, y, width, height));
}

CControl::~CControl() {
    sMap->erase(mControlId);
    sSet->erase(this);
}

define_reflectable_class_function(CControl, setControlId, "setter;args:iden")
void CControl::setControlId(const std::string &iden) {
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
    attachControl(&mPreviousControl, iden);
}

define_reflectable_class_function(CControl, setNextControl, "setter;args:iden")
void CControl::setNextControl(const std::string &iden) {
    attachControl(&mNextControl, iden);
}

define_reflectable_class_function(CControl, previousControl, "getter")
std::string CControl::previousControl() {
    CControl *control = checkControl(&mPreviousControl);
    if (control) {
        return control->controlId();
    }
    return "";
}

define_reflectable_class_function(CControl, nextControl, "getter")
std::string CControl::nextControl() {
    CControl *control = checkControl(&mNextControl);
    if (control) {
        return control->controlId();
    }
    return "";
}

define_reflectable_class_function(CControl, transferFocusToPrevious)
void CControl::transferFocusToPrevious() {
    CControl *previous = checkControl(&mPreviousControl);
    if (previous) {
        previous->becomeFocusResponder();
    }
}

define_reflectable_class_function(CControl, transferFocusToNext)
void CControl::transferFocusToNext() {
    CControl *next = checkControl(&mNextControl);
    if (next) {
        next->becomeFocusResponder();
    }
}

define_reflectable_class_function(CControl, transferFocusToAny)
void CControl::transferFocusToAny() {
    //try transferring to next:
    CControl *next = checkControl(&mNextControl);
    if (next) {
        next->becomeFocusResponder();
    }

    if (next && next->isFocusResponder()) {
        return;
    }

    //try transferring to previous.
    CControl *previous = checkControl(&mPreviousControl);
    if (previous) {
        previous->becomeFocusResponder();
    }
}

void CControl::onDrawForeground(float width, float height) {
    if (!isFocusResponder()) {
        return;
    }
    
    //draw focus frame:
    MContextSelectRGBA(MColor::BlackRGBA);
    const float thick = 2.0f;
    
    MContextDrawRect(0,              0, width, thick ); //top.
    MContextDrawRect(0, height - thick, width, thick ); //bottom.
    MContextDrawRect(0,              0, thick, height); //left.
    MContextDrawRect(width - thick,  0, thick, height); //right.
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

void CControl::attachControl(CControl **target, const std::string &iden) {
    auto it = sMap->find(iden);

    //NOTE: record the reference to the control instead of id. because id may changes.
    if (it != sMap->end() && it->second != this) {
        *target = it->second;
    } else {
        *target = nullptr;
    }
}

CControl *CControl::checkControl(CControl **target) {
    if (!*target) {
        return nullptr;
    }

    //the control has been released.
    if (sSet->find(*target) == sSet->end()) {
        *target = nullptr;
    }

    return *target;
}
