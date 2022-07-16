#pragma once

#include "cviewcontroller.h"

m_class(CNavigationController, CViewController) {
public:
    CNavigationController();

public:
    bool containsPageController(const CViewController::ptr &controller);
    CViewController::ptr topPageController();

    void pushPageController (const CViewController::ptr &controller);
    void popToPageController(const CViewController::ptr &controller);
    void popPageController  ();

protected:
    void onViewLoad() override;

private:
    void showBackPageController();
    void hideBackPageController();

    void layoutPageControllers();

private:
    MVector<CViewController::ptr>::ptr mPageControllers;
};
