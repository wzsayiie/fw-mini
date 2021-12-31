#pragma once

#include "cboot.h"

c_class(CUIResponder) {
    
public:
    virtual ~CUIResponder();
    
    void asRootResponder();
    
    void becomeFirstResponder();
    void resignFirstResponder();
    bool isFirstResponder();
    
    virtual bool canRespondWindowTouch(float x, float y) { return false; }
    virtual bool canRespondText() { return false; }
    virtual bool canRespondKey () { return false; }
    
    virtual CUIResponder *findResponder(std::function<bool (CUIResponder *)> fit) { return nullptr; }

    void setAcceptMouseMove(bool accept);
    
protected:
    virtual void onBecomeFirstResponder() {}
    virtual void onResignFirstResponder() {}

    //NOTE: here "x" and "y" are the coordinates of the window.
    virtual void onWindowTouchBegin(float x, float y) {}
    virtual void onWindowTouchMove (float x, float y) {}
    virtual void onWindowTouchEnd  (float x, float y) {}
    
    virtual void onTextInput(const std::string &string, bool enter) {}
    virtual void onKeyDown(MKey key) {}

    virtual void onWindowMouseMove(float x, float y) {}
    
private:
    static void handleWindowEvent(MObject *);
    
    static CUIResponder *findFirstResponder(bool refind, std::function<bool (CUIResponder *)> fit);
    
    static void handleWindowTouchBegin();
    static void handleWindowTouchMove();
    static void handleWindowTouchEnd();
    static void handleWindowText();
    static void handleWindowKeyDown();
    static void handleWindowMouseMove();
};
