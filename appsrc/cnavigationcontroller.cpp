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
        CViewController::ptr oldTop = mPageControllers->back();
        oldTop->removeFromParentController();
    }

    //show new top.
    controller->setNavigationController(this);
    mPageControllers->push_back(controller);
    addChildController(controller);
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

    CViewController::ptr oldTop = mPageControllers->back();
    oldTop->removeFromParentController();

    mPageControllers->erase(target + 1, mPageControllers->end());

    //show new top.
    addChildController(*target);
}

define_reflectable_class_function(CNavigationController, popPageController)
void CNavigationController::popPageController() {
    //remove old top.
    if (!mPageControllers->empty()) {
        CViewController::ptr oldTop = mPageControllers->back();
        mPageControllers->pop_back();
        
        oldTop->setNavigationController(nullptr);
        oldTop->removeFromParentController();
    }

    //show new top.
    if (!mPageControllers->empty()) {
        CViewController::ptr newTop = mPageControllers->back();
        addChildController(newTop);
    }
}

void CNavigationController::onViewLoad() {
    view()->setLayoutDelegate(MF(this, &CNavigationController::layoutPageControllers));
}

void CNavigationController::layoutPageControllers() {
    if (!mPageControllers->empty()) {
        CViewController::ptr top = mPageControllers->back();
        top->view()->setFrame(view()->bounds());
    }
}
