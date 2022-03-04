#include "mgraphics.h"

//pixel scale:
//

static float sPixelScale = 1.0f;

define_reflectable_function(MSetPixelScale)
void MSetPixelScale(float scale) {
    sPixelScale = scale;
}

define_reflectable_function(MGetPixelScale)
float MGetPixelScale() {
    return sPixelScale;
}

float _MPointFromPixelCaster::operator<<(float px) {
    return px / sPixelScale;
}

float _MPixelFromPointCaster::operator<<(float pt) {
    return pt * sPixelScale;
}
