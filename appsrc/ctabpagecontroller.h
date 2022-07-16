#pragma once

#include "cviewcontroller.h"

m_class(CTabPageController, CViewController) {
public:
    CTabPageController();

private:
    MVector<CViewController::ptr>::ptr mContentControllers;
};
