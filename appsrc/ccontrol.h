#pragma once

#include "cview.h"

typedef std::shared_ptr<class CControl> CControlRef;

class CControl : public CView {

public:
    CControl(float x, float y, float width, float height);
};
