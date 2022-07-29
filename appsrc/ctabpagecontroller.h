#pragma once

#include "cviewcontroller.h"

//tab page controller delegation:

m_class(CTabPageControllerDelegation, CObject) {
public:
    void setPagesChangeTarget(const MFunction<void ()>::ptr &target);
    MFunction<void ()>::ptr pagesChangeTarget();
    
    void pagesChange();
    
private:
    MFunction<void ()>::ptr mPagesChangeTarget;
};

//tab page controller:

m_class(CTabPageController, CViewController) {
public:
    static const int NoneIndex = -1;

public:
    CTabPageController();
    
public:
    void setDelegation(const CTabPageControllerDelegation::ptr &delegation);
    CTabPageControllerDelegation::ptr delegation();
    
    void setPages(const MVector<CViewController::ptr>::ptr &pages);

    int indexOfPage(const CViewController::ptr &page);
    CViewController::ptr pageAtIndex(int index);
    
    void selectPage(const CViewController::ptr &page);
    void selectIndex(int index);
    
    CViewController::ptr selectedPage();
    int selectedIndex();

protected:
    void onViewLoad() override;

private:
    void layoutPages();
    void showPage(const CViewController::ptr &page);
    void hidePage(const CViewController::ptr &page);

private:
    CTabPageControllerDelegation::ptr mDelegation;
    MVector<CViewController::ptr>::ptr mPages;
    int mSelectedIndex = NoneIndex;
};
