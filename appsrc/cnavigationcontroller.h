#pragma once

#include "cviewcontroller.h"

//navigation controller delegate:

m_class(CNavigationControllerDelegate, CObject) {
};

//navigation controller:

m_class(CNavigationController, CViewController) {
public:
    CNavigationController();

public:
    void pushPageController (const CViewController::ptr &controller);
    void popToPageController(const CViewController::ptr &controller);
    void popPageController  ();
    
    CViewController::ptr topPageController();
};
