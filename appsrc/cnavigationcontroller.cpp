#include "cnavigationcontroller.h"

//navigation controller delegation:

define_reflectable_class_function(CNavigationControllerDelegation, setPagesChangeTarget, "setter;args:target")
void CNavigationControllerDelegation::setPagesChangeTarget(const MFunction<void ()>::ptr &target) {
    mPagesChangeTarget = target;
}

define_reflectable_class_function(CNavigationControllerDelegation, pagesChangeTarget, "getter")
MFunction<void ()>::ptr CNavigationControllerDelegation::pagesChangeTarget() {
    return mPagesChangeTarget;
}

define_reflectable_class_function(CNavigationControllerDelegation, pagesChange)
void CNavigationControllerDelegation::pagesChange() {
    if (mPagesChangeTarget) {
        mPagesChangeTarget->call();
    }
}

//navigation controller:

CNavigationController::CNavigationController() {
}

define_reflectable_class_function(CNavigationController, setDelegation, "setter:args:delegation")
void CNavigationController::setDelegation(const CNavigationControllerDelegation::ptr &delegation) {
}

define_reflectable_class_function(CNavigationController, delegation, "getter")
CNavigationControllerDelegation::ptr CNavigationController::delegation() {
    return nullptr;
}

define_reflectable_class_function(CNavigationController, pushPageController, "args:controller")
void CNavigationController::pushPageController(const CViewController::ptr &controller) {
}

define_reflectable_class_function(CNavigationController, popToPageController, "args:controller")
void CNavigationController::popToPageController(const CViewController::ptr &controller) {
}

define_reflectable_class_function(CNavigationController, popPageController)
void CNavigationController::popPageController() {
}

define_reflectable_class_function(CNavigationController, topPageController, "getter")
CViewController::ptr CNavigationController::topPageController() {
    return nullptr;
}
