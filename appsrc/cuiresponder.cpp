#include "cuiresponder.h"

static MLambda      *sWindowListener  = nullptr;
static CUIResponder *sRootResponder   = nullptr;
static CUIResponder *sFirstResponder  = nullptr;
static CUIResponder *sActiveResponder = nullptr;

CUIResponder::~CUIResponder() {
    if (sRootResponder   == this) { sRootResponder  = nullptr; }
    if (sFirstResponder  == this) { sFirstResponder = nullptr; }
    if (sActiveResponder == this) { sRootResponder  = nullptr; }
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

void CUIResponder::handleWindowEvent(MObject *, MObject *) {
    MWindowEvent event = MWindowCurrentEvent();
    switch (event) {
        case MWindowEvent_TouchBegin: handleWindowTouchBegin(); break;
        case MWindowEvent_TouchMove : handleWindowTouchMove (); break;
        case MWindowEvent_TouchEnd  : handleWindowTouchEnd  (); break;
        case MWindowEvent_TextBox   : handleWindowText      (); break;
        case MWindowEvent_KeyDown   : handleWindowKeyDown   (); break;
        default:;
    }
}

void CUIResponder::handleWindowTouchBegin() {
    float x = MWindowTouchX();
    float y = MWindowTouchY();

    CUIResponder *responder = nullptr;
    if (sFirstResponder && sFirstResponder->canRespondTouch(x, y)) {
        responder = sFirstResponder;

    } else if (sRootResponder) {
        responder = sRootResponder->findResponderForTouch(x, y);
        if (responder) {
            responder->becomeFirstResponder();
        }
    }

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
    sActiveResponder->onWindowTouchMove(x, y);

    sActiveResponder = nullptr;
}

void CUIResponder::handleWindowText() {
    CUIResponder *responder = nullptr;
    if (sFirstResponder && sFirstResponder->canRespondText()) {
        responder = sFirstResponder;

    } else if (sRootResponder) {
        responder = sRootResponder->findResponderForText();
        if (responder) {
            responder->becomeFirstResponder();
        }
    }

    if (responder) {
        MString *string = MWindowTextBoxString();
        bool enter = MWindowTextBoxEnter();
        responder->onTextInput(string->u8Bytes(), enter);
    }
}

void CUIResponder::handleWindowKeyDown() {
    CUIResponder *responder = nullptr;
    if (sFirstResponder && sFirstResponder->canRespondKey()) {
        responder = sFirstResponder;

    } else if (sRootResponder) {
        responder = sRootResponder->findResponderForKey();
        if (responder) {
            responder->becomeFirstResponder();
        }
    }

    if (responder) {
        MKey key = MWindowActiveKey();
        responder->onKeyDown(key);
    }
}
