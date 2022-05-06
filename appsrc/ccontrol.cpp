#include "ccontrol.h"

CControl::CControl() {
    init();
}

CControl::CControl(float x, float y, float width, float height) {
    setFrame(MRect::from(x, y, width, height));
    init();
}

void CControl::init() {
    setTouchable(true);
    setAcceptMouseMove(true);
    setAcceptWheel(true);
}
