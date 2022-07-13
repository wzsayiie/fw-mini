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
define_reflectable_class_function(CControl, setNextControl    , "setter;args:iden")

void CControl::setPreviousControl(const std::string &iden) { setControl(&mPreviousControl, iden); }
void CControl::setNextControl    (const std::string &iden) { setControl(&mNextControl    , iden); }

define_reflectable_class_function(CControl, previousControl, "getter")
define_reflectable_class_function(CControl, nextControl    , "getter")

std::string CControl::previousControl() { return controlIdOf(&mPreviousControl); }
std::string CControl::nextControl    () { return controlIdOf(&mNextControl    ); }

define_reflectable_class_function(CControl, transferFocusControl)
void CControl::transferFocusControl() {
    transfer({ &mNextControl, &mPreviousControl });

    //abandon the focus if it has not been transferred.
    if (isFocusResponder()) {
        resignFocusResponder();
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

void CControl::onKey(MKey key) {
    switch (key) {
        case MKey::Left : transfer({ &mPreviousControl }); break;
        case MKey::Up   : transfer({ &mPreviousControl }); break;
        case MKey::Right: transfer({ &mNextControl     }); break;
        case MKey::Down : transfer({ &mNextControl     }); break;
        case MKey::Tab  : transfer({ &mNextControl     }); break;

        //pass the event to subclass.
        default: onControlKey(key);
    }
}

define_reflectable_class_function(CControl, onControlKey, "virtual;args:key")
void CControl::onControlKey(MKey key) {
    implement_injectable_function(void, key)
}

void CControl::setControl(CControl **target, const std::string &iden) {
    auto it = sMap->find(iden);

    //NOTE: record the reference to the control instead of id. cause id may change.
    if (it != sMap->end() && it->second != this) {
        *target = it->second;
    } else {
        *target = nullptr;
    }
}

std::string CControl::controlIdOf(CControl **target) {
    if (!*target) {
        return "";
    }

    //the control has been released.
    if (sSet->find(*target) == sSet->end()) {
        *target = nullptr;
        return "";
    }

    return (*target)->controlId();
}

void CControl::transfer(const std::initializer_list<CControl **> &targets) {
    for (auto &target : targets) {
        if (!*target) {
            continue;
        }

        //the control has been released.
        if (sSet->find(*target) == sSet->end()) {
            *target = nullptr;
            continue;
        }
        
        (*target)->becomeFocusResponder();
        break;
    }
}
