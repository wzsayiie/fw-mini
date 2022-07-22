#include "cnavigationcontroller.h"

//navigation controller delegate:

define_reflectable_class_function(CNavigationControllerDelegate, setPagesChangeTarget, "setter;args:target")
void CNavigationControllerDelegate::setPagesChangeTarget(const MFunction<void ()>::ptr &target) {
    mPagesChangeTarget = target;
}

define_reflectable_class_function(CNavigationControllerDelegate, pagesChangeTarget, "getter")
MFunction<void ()>::ptr CNavigationControllerDelegate::pagesChangeTarget() {
    return mPagesChangeTarget;
}

define_reflectable_class_function(CNavigationControllerDelegate, pagesChange)
void CNavigationControllerDelegate::pagesChange() {
    if (mPagesChangeTarget) {
        mPagesChangeTarget->call();
    }
}

//navigation controller:

CNavigationController::CNavigationController() {
}

define_reflectable_class_function(CNavigationController, setDelegate, "setter:args:delegate")
void CNavigationController::setDelegate(const CNavigationControllerDelegate::ptr &delegate) {
}

define_reflectable_class_function(CNavigationController, delegate, "getter")
CNavigationControllerDelegate::ptr CNavigationController::delegate() {
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
