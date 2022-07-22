#include "ctabpagecontroller.h"

//tab page controller delegate:

define_reflectable_class_function(CTabPageControllerDelegate, setPagesChangeTarget, "setter;args:target")
void CTabPageControllerDelegate::setPagesChangeTarget(const MFunction<void ()>::ptr &target) {
    mPagesChangeTarget = target;
}

define_reflectable_class_function(CTabPageControllerDelegate, pagesChangeTarget, "getter")
MFunction<void ()>::ptr CTabPageControllerDelegate::pagesChangeTarget() {
    return mPagesChangeTarget;
}

define_reflectable_class_function(CTabPageControllerDelegate, pagesChange)
void CTabPageControllerDelegate::pagesChange() {
    if (mPagesChangeTarget) {
        mPagesChangeTarget->call();
    }
}

//tab page controller:

CTabPageController::CTabPageController() {
}

define_reflectable_class_function(CTabPageController, setDelegate, "setter:args:delegate")
void CTabPageController::setDelegate(const CTabPageControllerDelegate::ptr &delegate) {
}

define_reflectable_class_function(CTabPageController, delegate, "getter")
CTabPageControllerDelegate::ptr CTabPageController::delegate() {
    return nullptr;
}

define_reflectable_class_function(CTabPageController, setPageControllers, "args:controllers")
void CTabPageController::setPageControllers(const MVector<CViewController::ptr>::ptr &controllers) {
}

define_reflectable_class_function(CTabPageController, indexOfPageController, "args:controller")
int CTabPageController::indexOfPageController(const CViewController::ptr &controller) {
    return -1;
}

define_reflectable_class_function(CTabPageController, pageControllerAtIndex, "args:index")
CViewController::ptr CTabPageController::pageControllerAtIndex(int index) {
    return nullptr;
}

define_reflectable_class_function(CTabPageController, selectPageController, "args:controller")
void CTabPageController::selectPageController(const CViewController::ptr &controller) {
}

define_reflectable_class_function(CTabPageController, selectPageIndex, "args:index")
void CTabPageController::selectPageIndex(int index) {
}

define_reflectable_class_function(CTabPageController, selectedPageController, "getter")
CViewController::ptr CTabPageController::selectedPageController() {
    return nullptr;
}

define_reflectable_class_function(CTabPageController, selectedPageIndex, "getter")
int CTabPageController::selectedPageIndex() {
    return -1;
}
