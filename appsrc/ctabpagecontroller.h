#pragma once

#include "cviewcontroller.h"

//tab page controller delegate:

m_class(CTabPageControllerDelegate, CObject) {
};

//tab page controller:

m_class(CTabPageController, CViewController) {
public:
    CTabPageController();
    
public:
    void setPageControllers(const MVector<CViewController::ptr>::ptr &controllers);

    int indexOfPageController(const CViewController::ptr &controller);
    CViewController::ptr pageControllerAtIndex(int index);
    
    void selectPageController(const CViewController::ptr &controller);
    void selectPageIndex(int index);
    
    CViewController::ptr selectedPageController();
    int selectedPageIndex();
};
