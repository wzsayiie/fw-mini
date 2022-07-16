#pragma once

#include "cviewcontroller.h"

c_class(CTabPageController, CViewController) {
public:
    CTabPageController();

private:
    MVector<CViewController::ptr>::ptr mContentControllers;
};
