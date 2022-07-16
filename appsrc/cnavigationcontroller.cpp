#include "cnavigationcontroller.h"

CNavigationController::CNavigationController() {
    mPageControllers = MVector<CViewController::ptr>::create();
}

define_reflectable_class_function(CNavigationController, containsPageController, "args:controller")
bool CNavigationController::containsPageController(const CViewController::ptr &controller) {
    if (controller) {
        auto target = std::find(mPageControllers->begin(), mPageControllers->end(), controller);
        return target != mPageControllers->end();
    }
    return false;
}

define_reflectable_class_function(CNavigationController, topPageController)
CViewController::ptr CNavigationController::topPageController() {
    if (!mPageControllers->empty()) {
        return mPageControllers->back();
    }
    return nullptr;
}

define_reflectable_class_function(CNavigationController, pushPageController, "args:controller")
void CNavigationController::pushPageController(const CViewController::ptr &controller) {
    if (!controller || containsPageController(controller)) {
        return;
    }

    //remove old top.
    if (!mPageControllers->empty()) {
        hideBackPageController();
    }

    //show new top:
    controller->setNavigationController(me());
    mPageControllers->push_back(controller);

    showBackPageController();
}

define_reflectable_class_function(CNavigationController, popToPageController, "args:controller")
void CNavigationController::popToPageController(const CViewController::ptr &controller) {
    auto target = std::find(mPageControllers->begin(), mPageControllers->end(), controller);
    if ( target == mPageControllers->end () ||
        *target == mPageControllers->back() )
    {
        return;
    }

    //remove the upper controllers.
    for (auto it = target + 1; it != mPageControllers->end(); ++it) {
        (*it)->setNavigationController(nullptr);
    }
    hideBackPageController();
    mPageControllers->erase(target + 1, mPageControllers->end());

    //show new top.
    showBackPageController();
}

define_reflectable_class_function(CNavigationController, popPageController)
void CNavigationController::popPageController() {
    //remove old top.
    if (!mPageControllers->empty()) {
        mPageControllers->back()->setNavigationController(nullptr);
        hideBackPageController();
        mPageControllers->pop_back();
    }

    //show new top.
    if (!mPageControllers->empty()) {
        showBackPageController();
    }
}

void CNavigationController::onViewLoad() {
    view()->setLayoutDelegate(MF(this, &CNavigationController::layoutPageControllers));
}

void CNavigationController::showBackPageController() {
    CViewController::ptr back = mPageControllers->back();
    addChildController(back);
}

void CNavigationController::hideBackPageController() {
    CViewController::ptr back = mPageControllers->back();

    //NOTE: remove focus if necessary.
    CResponder::ptr focus = CResponder::focusResponder();
    if (back->existResponder(focus)) {
        focus->resignFocusResponder();
    }

    back->removeFromParentController();
}

void CNavigationController::layoutPageControllers() {
    if (!mPageControllers->empty()) {
        CViewController::ptr back = mPageControllers->back();
        back->view()->setFrame(view()->bounds());
    }
}
