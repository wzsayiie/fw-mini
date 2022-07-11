#include "ccontrol.h"

static dash::lazy<std::map<std::string, CControl *>> sMap;
static dash::lazy<std::set<CControl *>> sSet;
static int sIdCount = 0;

CControl::CControl() {
    mControlId = std::to_string(++sIdCount);
    sMap->insert({ mControlId, this });
    sSet->insert(this);
    
    setTouchable(true);
    setAcceptMouseMove(true);
    setAcceptWheel(true);
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

define_reflectable_class_function(CControl, transferFocusControl)
void CControl::transferFocusControl() {
    transfer({
        &mRightControl, &mNextControl ,
        &mLeftControl , &mFrontControl
    });

    //abandon the focus if it has not been transferred.
    if (isFocusResponder()) {
        resignFocusResponder();
    }
}

define_reflectable_class_function(CControl, setFrontControl, "setter;args:iden")
define_reflectable_class_function(CControl, setNextControl , "setter;args:iden")
define_reflectable_class_function(CControl, setLeftControl , "setter;args:iden")
define_reflectable_class_function(CControl, setRightControl, "setter;args:iden")

void CControl::setFrontControl(const std::string &iden) { setControl(&mFrontControl, iden); }
void CControl::setNextControl (const std::string &iden) { setControl(&mNextControl , iden); }
void CControl::setLeftControl (const std::string &iden) { setControl(&mLeftControl , iden); }
void CControl::setRightControl(const std::string &iden) { setControl(&mRightControl, iden); }

define_reflectable_class_function(CControl, frontControl, "getter")
define_reflectable_class_function(CControl, nextControl , "getter")
define_reflectable_class_function(CControl, leftControl , "getter")
define_reflectable_class_function(CControl, rightControl, "getter")

std::string CControl::frontControl() { return controlIdOf(&mFrontControl); }
std::string CControl::nextControl () { return controlIdOf(&mNextControl ); }
std::string CControl::leftControl () { return controlIdOf(&mLeftControl ); }
std::string CControl::rightControl() { return controlIdOf(&mRightControl); }

void CControl::onDrawForeground(float width, float height) {
    if (!isFocusResponder()) {
        return;
    }
    
    //mark focus responder:
    MContextSelectRGBA(MColor::BlackRGBA);
    const float thick = 2.0f;
    
    MContextDrawRect(0,              0, width, thick ); //top.
    MContextDrawRect(0, height - thick, width, thick ); //bottom.
    MContextDrawRect(0,              0, thick, height); //left.
    MContextDrawRect(width - thick,  0, thick, height); //right.
}

bool CControl::canRespondKey() {
    return true;
}

void CControl::onKey(MKey key) {
    switch (key) {
        case MKey::Down : transfer({ &mNextControl , &mRightControl }); break;
        case MKey::Right: transfer({ &mRightControl, &mNextControl  }); break;
        case MKey::Up   : transfer({ &mFrontControl, &mLeftControl  }); break;
        case MKey::Left : transfer({ &mLeftControl , &mFrontControl }); break;
        
        case MKey::Tab: {
            transfer({
                &mRightControl, &mNextControl ,
                &mLeftControl , &mFrontControl
            });
            break;
        }

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
    if (it != sMap->end()) {
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

void CControl::transfer(const std::initializer_list<CControl **> &ids) {
    for (auto &iden : ids) {
        //the control has been released.
        if (sSet->find(*iden) == sSet->end()) {
            *iden = nullptr;
            continue;
        }
        
        (*iden)->becomeFocusResponder();
        break;
    }
}
