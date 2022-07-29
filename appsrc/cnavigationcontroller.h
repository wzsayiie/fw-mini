#pragma once

#include "cviewcontroller.h"

//navigation controller delegation:

m_class(CNavigationControllerDelegation, CObject) {
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
    void setDelegation(const CNavigationControllerDelegation::ptr &delegation);
    CNavigationControllerDelegation::ptr delegation();
    
    void pushPageController (const CViewController::ptr &controller);
    void popToPageController(const CViewController::ptr &controller);
    void popPageController  ();
    
    CViewController::ptr topPageController();
};
