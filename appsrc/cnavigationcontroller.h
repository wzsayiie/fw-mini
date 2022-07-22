#pragma once

#include "cviewcontroller.h"

//navigation controller delegate:

m_class(CNavigationControllerDelegate, CObject) {
public:
    void setPagesChangeTarget(const MFunction<void ()>::ptr &target);
    MFunction<void ()>::ptr pagesChangeTarget();
    
    void pagesChange();
    
private:
    MFunction<void ()>::ptr mPagesChangeTarget;
};

//navigation controller:

m_class(CNavigationController, CViewController) {
public:
    CNavigationController();

public:
    void setDelegate(const CNavigationControllerDelegate::ptr &delegate);
    CNavigationControllerDelegate::ptr delegate();
    
    void pushPageController (const CViewController::ptr &controller);
    void popToPageController(const CViewController::ptr &controller);
    void popPageController  ();
    
    CViewController::ptr topPageController();
};
