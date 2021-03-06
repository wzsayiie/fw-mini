#include "cuiresponder.h"

static MLambda      *sWindowListener  = nullptr;
static CUIResponder *sRootResponder   = nullptr;
static CUIResponder *sFirstResponder  = nullptr;
static CUIResponder *sActiveResponder = nullptr;

static def_singleton(sMouseMoveResponders, std::set<CUIResponder *>())

CUIResponder::~CUIResponder() {
    if (sRootResponder   == this) { sRootResponder  = nullptr; }
    if (sFirstResponder  == this) { sFirstResponder = nullptr; }
    if (sActiveResponder == this) { sRootResponder  = nullptr; }

    sMouseMoveResponders().erase(this);
}

void CUIResponder::asRootResponder() {
    if (!sWindowListener) {
        sWindowListener = MLambdaCreate(handleWindowEvent, nullptr);
        MWindowAddListener(sWindowListener);
    }
    sRootResponder = this;
}

void CUIResponder::becomeFirstResponder() {
    if (this == sFirstResponder) {
        return;
    }

    CUIResponder *lastFirstResponder = sFirstResponder;
    sFirstResponder = this;

    if (lastFirstResponder) {
        lastFirstResponder->onResignFirstResponder();
    }
    onBecomeFirstResponder();
}

void CUIResponder::resignFirstResponder() {
    if (this == sFirstResponder) {
        sFirstResponder = nullptr;
        onResignFirstResponder();
    }
}

bool CUIResponder::isFirstResponder() {
    return this == sFirstResponder;
}

void CUIResponder::setAcceptMouseMove(bool accept) {
    if (accept) {
        sMouseMoveResponders().insert(this);
    } else {
        sMouseMoveResponders().erase(this);
    }
}

void CUIResponder::handleWindowEvent(MObject *) {
    MWindowEvent event = MWindowCurrentEvent();
    switch (event) {
        case MWindowEvent_TouchBegin: handleWindowTouchBegin(); break;
        case MWindowEvent_TouchMove : handleWindowTouchMove (); break;
        case MWindowEvent_TouchEnd  : handleWindowTouchEnd  (); break;
        case MWindowEvent_MouseWheel: handleWindowMouseWheel(); break;
        case MWindowEvent_MouseMove : handleWindowMouseMove (); break;
        case MWindowEvent_TextBox   : handleWindowText      (); break;
        case MWindowEvent_KeyDown   : handleWindowKeyDown   (); break;

        default:;
    }
}

CUIResponder *CUIResponder::findFirstResponder(bool refind, const CLambda<bool (CUIResponder *)> &fit) {
    CUIResponder *responder = nullptr;

    if (!refind && sFirstResponder && fit(sFirstResponder)) {
        responder = sFirstResponder;

    } else if (sRootResponder) {
        responder = sRootResponder->findResponder(fit);
        if (responder) {
            responder->becomeFirstResponder();
        }
    }
    
    return responder;
}

void CUIResponder::handleWindowTouchBegin() {
    float x = MWindowTouchX();
    float y = MWindowTouchY();

    CUIResponder *responder = findFirstResponder(true, [x, y](CUIResponder *candidate) {
        return candidate->canRespondWindowTouch(x, y);
    });

    if (responder) {
        sActiveResponder = responder;
        sActiveResponder->onWindowTouchBegin(x, y);
    }
}

void CUIResponder::handleWindowTouchMove() {
    if (!sActiveResponder) {
        return;
    }

    float x = MWindowTouchX();
    float y = MWindowTouchY();
    sActiveResponder->onWindowTouchMove(x, y);
}

void CUIResponder::handleWindowTouchEnd() {
    if (!sActiveResponder) {
        return;
    }

    float x = MWindowTouchX();
    float y = MWindowTouchY();
    sActiveResponder->onWindowTouchEnd(x, y);

    sActiveResponder = nullptr;
}

void CUIResponder::handleWindowMouseWheel() {
    float x = MWindowMouseX();
    float y = MWindowMouseY();
    
    CUIResponder *responder = findFirstResponder(false, [x, y](CUIResponder *candidate) {
        return candidate->canRespondWindowWheel(x, y);
    });
    
    if (responder) {
        float delta = MWindowWheelDelta();
        responder->onWindowMouseWheel(x, y, delta);
    }
}

void CUIResponder::handleWindowMouseMove() {
    float x = MWindowMouseX();
    float y = MWindowMouseY();

    for (auto &it : sMouseMoveResponders()) {
        it->onWindowMouseMove(x, y);
    }
}

void CUIResponder::handleWindowText() {
    CUIResponder *responder = findFirstResponder(false, [](CUIResponder *candidate) {
        return candidate->canRespondText();
    });

    if (responder) {
        MString *string = MWindowTextBoxString();
        bool enter = MWindowTextBoxEnter();
        responder->onTextInput(MStringU8Chars(string), enter);
    }
}

void CUIResponder::handleWindowKeyDown() {
    CUIResponder *responder = findFirstResponder(false, [](CUIResponder *candidate) {
        return candidate->canRespondKey();
    });

    if (responder) {
        MKey key = MWindowActiveKey();
        responder->onKeyDown(key);
    }
}
