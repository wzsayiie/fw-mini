#include "ccontrol.h"

static dash::lazy<std::map<std::string, CControl *>> sResponders;
static int sResponderIdCount = 0;

CControl::CControl() {
    mResponderId = std::to_string(++sResponderIdCount);
    sResponders->insert({ mResponderId, this });
    
    setTouchable(true);
    setAcceptMouseMove(true);
    setAcceptWheel(true);
}

CControl::CControl(float x, float y, float width, float height): CControl() {
    setFrame(MRect::from(x, y, width, height));
}

CControl::~CControl() {
    sResponders->erase(mResponderId);
}

define_reflectable_class_function(CControl, setResponderId, "setter;args:iden")
void CControl::setResponderId(const std::string &iden) {
    if (iden.empty()) {
        return;
    }
    
    sResponders->erase(mResponderId);
    sResponders->insert({ iden, this });
    mResponderId = iden;
}

define_reflectable_class_function(CControl, responderId, "getter")
std::string CControl::responderId() {
    return mResponderId;
}

define_reflectable_class_function(CControl, transferFocusResponder)
void CControl::transferFocusResponder() {
    transfer({ mRightResponder, mNextResponder });
}

define_reflectable_class_function(CControl, setFrontResponder, "setter;args:iden")
define_reflectable_class_function(CControl, setNextResponder , "setter;args:iden")
define_reflectable_class_function(CControl, setLeftResponder , "setter;args:iden")
define_reflectable_class_function(CControl, setRightResponder, "setter;args:iden")

void CControl::setFrontResponder(const std::string &iden) { mFrontResponder = iden; }
void CControl::setNextResponder (const std::string &iden) { mNextResponder  = iden; }
void CControl::setLeftResponder (const std::string &iden) { mLeftResponder  = iden; }
void CControl::setRightResponder(const std::string &iden) { mRightResponder = iden; }

define_reflectable_class_function(CControl, frontResponder, "getter")
define_reflectable_class_function(CControl, nextResponder , "getter")
define_reflectable_class_function(CControl, leftResponder , "getter")
define_reflectable_class_function(CControl, rightResponder, "getter")

std::string CControl::frontResponder() { return mFrontResponder; }
std::string CControl::nextResponder () { return mNextResponder ; }
std::string CControl::leftResponder () { return mLeftResponder ; }
std::string CControl::rightResponder() { return mRightResponder; }

void CControl::onDrawForeground(float width, float height) {
    if (!isFocusResponder()) {
        return;
    }
    
    //mark focus responder:
    MContextSelectRGBA(MColor::BlackRGBA);
    const float thick = 2;
    
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
        case MKey::Tab  : transfer({ mRightResponder, mNextResponder  }); break;
        case MKey::Down : transfer({ mNextResponder , mRightResponder }); break;
        case MKey::Right: transfer({ mRightResponder, mNextResponder  }); break;
        case MKey::Up   : transfer({ mFrontResponder                  }); break;
        case MKey::Left : transfer({ mLeftResponder                   }); break;
        
        //pass the event to subclass.
        default: onControlKey(key);
    }
}

define_reflectable_class_function(CControl, onControlKey, "virtual;args:key")
void CControl::onControlKey(MKey key) {
    implement_injectable_function(void, key)
}

void CControl::transfer(const std::initializer_list<std::string> &ids) {
    for (auto &iden : ids) {
        if (iden.empty()) {
            continue;
        }
        
        auto it = sResponders->find(iden);
        if (it == sResponders->end()) {
            continue;
        }
        
        it->second->becomeFocusResponder();
        break;
    }
}
