#include "mcontext.h"

static float sOffsetX = 0;
static float sOffsetY = 0;

void MContextSetOffset(float x, float y) {
    sOffsetX = x;
    sOffsetY = y;
}

void MContextSelectColor(MColor color) {
    MWindowSelectColor(color);
}

void MContextSelectImage(MImage *image) {
    MWindowSelectImage(image);
}

void MContextSelectString(MString *string) {
    MWindowSelectString(string);
}

void MContextSelectFontSize(float size) {
    MWindowSelectFontSize(size);
}

void MContextSelectHAlign(MHAlign a) { MWindowSelectHAlign(a); }
void MContextSelectVAlign(MVAlign a) { MWindowSelectVAlign(a); }

void MContextDrawRect(float x, float y, float width, float height) {
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

void MContextDrawImage(float x, float y, float width, float height) {
    float top    = sOffsetY + y;
    float bottom = sOffsetY + y + height;
    float left   = sOffsetX + x;
    float right  = sOffsetX + x + width;

    MWindowSelectPoint0(left , top);
    MWindowSelectPoint1(right, bottom);

    MWindowDrawImage();
}

void MContextDrawString(float x, float y, float width, float height) {
    float top    = sOffsetY + y;
    float bottom = sOffsetY + y + height;
    float left   = sOffsetX + x;
    float right  = sOffsetX + x + width;

    MWindowSelectPoint0(left , top);
    MWindowSelectPoint1(right, bottom);

    MWindowDrawLabel();
}
