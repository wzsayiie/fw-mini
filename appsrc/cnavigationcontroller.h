#pragma once

#include "cviewcontroller.h"

//navigation controller delegation:

m_class(CNavigationControllerDelegation, CObject) {
public:
    void setPagesChangeTarget(const MFunction<void ()>::ptr &target);
    MFunction<void ()>::ptr pagesChangeTarget();
    
    void pagesChange();
    
private:
    void on_dispose() override;

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
    
    void pushPage (const CViewController::ptr &page);
    void popToPage(const CViewController::ptr &page);
    void popPage  ();
    
    CViewController::ptr topPage();

protected:
    void onViewLoad() override;

private:
    void layoutPages();
    void showPage(const CViewController::ptr &page);
    void hidePage(const CViewController::ptr &page);

private:
    void on_dispose() override;

private:
    CNavigationControllerDelegation::ptr mDelegation;
    MVector<CViewController::ptr>::ptr mPages;
};
