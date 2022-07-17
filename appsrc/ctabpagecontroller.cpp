#include "ctabpagecontroller.h"

CTabPageController::CTabPageController() {
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
