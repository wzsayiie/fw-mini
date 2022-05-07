#include "ccontrol.h"

CControl::CControl() {
    setTouchable(true);
    setAcceptMouseMove(true);
    setAcceptWheel(true);
}

CControl::CControl(float x, float y, float width, float height): CControl() {
    setFrame(MRect::from(x, y, width, height));
}
