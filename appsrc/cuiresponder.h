#pragma once

#include <memory>
#include <string>
#include "mhostui.h"

typedef std::shared_ptr<class CUIResponder> CUIResponderRef;

class CUIResponder {
    
public:
    virtual ~CUIResponder();
    
    void asRootResponder();
    
    void becomeFirstResponder();
    void resignFirstResponder();
    bool isFirstResponder();
    
    virtual bool canRespondTouch(float x, float y) { return false; }
    virtual bool canRespondText () { return false; }
    virtual bool canRespondKey  () { return false; }
    
    virtual CUIResponder *findResponderForTouch(float x, float y) { return nullptr; }
    virtual CUIResponder *findResponderForText () { return nullptr; }
    virtual CUIResponder *findResponderForKey  () { return nullptr; }
    
protected:
    virtual void onBecomeFirstResponder() {}
    virtual void onResignFirstResponder() {}

    //NOTE: here "x" and "y" are the coordinates of the window.
    virtual void onWindowTouchBegin(float x, float y) {}
    virtual void onWindowTouchMove (float x, float y) {}
    virtual void onWindowTouchEnd  (float x, float y) {}
    
    virtual void onTextInput(const std::string &string, bool enter) {}
    
    virtual void onKeyDown(MKey key) {}
    
private:
    static void handleWindowEvent(MObject *, MObject *);
    
    static void handleWindowTouchBegin();
    static void handleWindowTouchMove();
    static void handleWindowTouchEnd();
    static void handleWindowText();
    static void handleWindowKeyDown();
};
