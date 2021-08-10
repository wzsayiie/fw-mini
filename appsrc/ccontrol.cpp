#include "ccontrol.h"

CControl::CControl(float x, float y, float w, float h): CView(x, y, w, h) {
    //controls are touchable by default.
    setTouchable(true);
}
