#include "ctabpagecontroller.h"

CTabPageController::CTabPageController() {
    mContentControllers = MVector<CViewController::ptr>::create();
}
