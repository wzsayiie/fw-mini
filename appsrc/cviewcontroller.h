#pragma once

#include <vector>
#include "cview.h"
#include "mtypes.h"

typedef std::shared_ptr<class CViewController> CViewControllerRef;

class CViewController : public CUIResponder {
    
public:
    ~CViewController();
    
    void asMainController();
    
    void addChildController(CViewControllerRef childController);
    void removeFromParentController();
    const std::vector<CViewControllerRef> &childControllers();
    CViewController *parentController();
    
    CViewRef view();
    virtual CViewRef loadView();
    
    virtual void viewLoad();
    virtual void viewShow();
    virtual void viewHide();
    
    virtual void viewResize(float width, float height);
    
private:
    void handleViewLoad();
    void handleViewShow();
    void handleViewHide();
    
    static void handleWindowEvent(MObject *, MObject *);
    
    void handleWindowLoad();
    void handleWindowResize();
    void handleWindowDraw();
    void handleWindowTouchBegin();
    void handleWindowTouchMove();
    void handleWindowTouchEnd();
    void handleWindowTextBox();
    void handleWindowKeyDown();
    
    std::vector<CViewControllerRef> mChildControllers;
    CViewController *mParentController = nullptr;
    
    bool mViewLoaded = false;
    bool mViewShown = false;
    CViewRef mView;
};
