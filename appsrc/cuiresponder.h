#pragma once

#include <memory>
#include <string>
#include "mhostui.h"

typedef std::shared_ptr<class CUIResponder> CUIResponderRef;

class CUIResponder {
    
public:
    virtual ~CUIResponder();
    
    void asRootResponder();
    
    virtual void becomeFirstResponder();
    virtual void resignFirstResponder();
    
    virtual bool canRespondTouch() { return false; }
    virtual bool canRespondText () { return false; }
    virtual bool canRespondKey  () { return false; }
    
    virtual CUIResponder *findTouchResponder(float x, float y) { return nullptr; }
    virtual CUIResponder *findTextResponder () { return nullptr; }
    virtual CUIResponder *findKeyResponder  () { return nullptr; }
    
protected:
    virtual void touchBegin(float x, float y) {}
    virtual void touchMove (float x, float y) {}
    virtual void touchEnd  (float x, float y) {}
    
    virtual void textInput(const std::string &string, bool enter) {}
    
    virtual void keyDown(MKey key) {}
    
private:
    static void handleWindowEvent(MObject *, MObject *);
    
    void handleTouchBegin();
    void handleTouchMove();
    void handleTouchEnd();
    void handleKeyDown();
    void handleText();
};
