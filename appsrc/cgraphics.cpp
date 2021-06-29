#include "cgraphics.h"

CColor::CColor(float red, float green, float blue, float alpha) {
    set(red, blue, green, alpha);
}

CColor::CColor() {
}

void CColor::set(float red, float green, float blue, float alpha) {
    mRed   = red  ;
    mGreen = green;
    mBlue  = blue ;
    mAlpha = alpha;
}

float CColor::red  () const { return mRed  ; }
float CColor::green() const { return mGreen; }
float CColor::blue () const { return mBlue ; }
float CColor::alpha() const { return mAlpha; }

MColor CColor::rgba() const {
    MColorPattern color = {0};
    
    color.red   = (uint8_t)(mRed   * 255);
    color.green = (uint8_t)(mGreen * 255);
    color.blue  = (uint8_t)(mBlue  * 255);
    color.alpha = (uint8_t)(mAlpha * 255);
    
    return color.rgba;
}

static float sOffsetX = 0;
static float sOffsetY = 0;

static CColor sColor;

void CContextSetOffset(float x, float y) {
    sOffsetX = x;
    sOffsetY = y;
}

void CContextSelectColor(const CColor &color) {
    sColor = color;
}

void CContextDrawEllipse(float x, float y, float width, float height) {
}

void CContextDrawRect(float x, float y, float width, float height) {
    MWindowSelectColor(sColor.rgba());
    
    float top    = sOffsetY + y;
    float bottom = sOffsetY + y + height;
    float left   = sOffsetX + x;
    float right  = sOffsetX + x + width;
    
    MWindowSelectPoint0(left , top);
    MWindowSelectPoint1(right, top);
    MWindowSelectPoint2(right, bottom);
    MWindowDrawTriangle();
    
    MWindowSelectPoint0(left , top);
    MWindowSelectPoint1(right, bottom);
    MWindowSelectPoint2(left , bottom);
    MWindowDrawTriangle();
}
