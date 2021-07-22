#pragma once

#include "cview.h"

typedef std::shared_ptr<class CViewController> CViewControllerRef;

class CViewController : public CUIResponder {
    
public:
    ~CViewController();
    
    void asRootController();
    
    void addChildController(CViewControllerRef childController);
    void removeFromParentController();
    const std::vector<CViewControllerRef> &childControllers();
    CViewController *parentController();
    
    CViewRef view();
    
    bool canRespondWindowTouch(float x, float y) override;
    CUIResponder *findResponder(std::function<bool (CUIResponder *)> fit) override;

protected:
    virtual CViewRef loadView();
    
    virtual void onViewLoad() {}
    virtual void onViewAppear() {}
    virtual void onViewDisappear() {}
    
    virtual void onTouchBegin(float x, float y) {}
    virtual void onTouchMove (float x, float y) {}
    virtual void onTouchEnd  (float x, float y) {}
    
    void onWindowTouchBegin(float x, float y) override;
    void onWindowTouchMove (float x, float y) override;
    void onWindowTouchEnd  (float x, float y) override;
    
private:
    static void handleWindowEvent(MObject *);
    
    void handleLoad();
    void handleAppear();
    void handleDisappear();

    std::vector<CViewControllerRef> mChildControllers;
    CViewController *mParentController = nullptr;
    
    bool mViewLoaded   = false;
    bool mViewAppeared = false;
    CViewRef mView;
};
