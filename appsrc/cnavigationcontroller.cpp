#include "cnavigationcontroller.h"

CNavigationController::CNavigationController() {
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
