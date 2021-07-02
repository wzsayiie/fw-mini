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

protected:
    virtual CViewRef loadView();
    
    virtual void onViewLoad() {}
    virtual void onViewAppear() {}
    virtual void onViewDisappear() {}
    
private:
    static void handleWindowEvent(MObject *, MObject *);
    
    void handleLoad();
    void handleAppear();
    void handleDisappear();

    std::vector<CViewControllerRef> mChildControllers;
    CViewController *mParentController = nullptr;
    
    bool mViewLoaded   = false;
    bool mViewAppeared = false;
    CViewRef mView;
};
