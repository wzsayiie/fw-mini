#include "mcontext.h"
#include <cmath>

static float sOffsetX   = 0;
static float sOffsetY   = 0;
static float sLineWidth = 0;

void MContextSetOffset(float x, float y) {
    sOffsetX = x;
    sOffsetY = y;
}

void MContextSelectColor   (MColor   color ) { MWindowSelectColor   (color ); }
void MContextSelectImage   (MImage  *image ) { MWindowSelectImage   (image ); }
void MContextSelectString  (MString *string) { MWindowSelectString  (string); }
void MContextSelectHAlign  (MHAlign  align ) { MWindowSelectHAlign  (align ); }
void MContextSelectVAlign  (MVAlign  align ) { MWindowSelectVAlign  (align ); }
void MContextSelectFontSize(float    size  ) { MWindowSelectFontSize(size  ); }

void MContextSelectLineWidth(float width) {
    sLineWidth = width;
}

void MContextDrawTriangle0(float x0, float y0) { MWindowSelectPoint0(sOffsetX + x0, sOffsetY + y0); }
void MContextDrawTriangle1(float x1, float y1) { MWindowSelectPoint0(sOffsetX + x1, sOffsetY + y1); }
void MContextDrawTriangle2(float x2, float y2) { MWindowSelectPoint0(sOffsetX + x2, sOffsetY + y2);
    MWindowDrawTriangle();
}

void MContextDrawFlatLine(float x0, float y0, float x1, float y1) {
}

void MContextDrawSquareLine(float x0, float y0, float x1, float y1) {
}

void MContextDrawRoundLine(float x0, float y0, float x1, float y1) {
}

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

void MContextDrawEllipse(float x, float y, float width, float height) {
    auto splits = (int)fminf(fmaxf(width, height) * 2, 64);
    if (splits < 4) {
        //if the number of splits is too few, use rectangle instead.
        MContextDrawRect(x, y, width, height);
        return;
    }
    
    float circle  = (float)(M_PI * 2);
    float step    = circle / splits;
    float wHalf   = width  / 2;
    float hHalf   = height / 2;
    float xCenter = sOffsetX + x + wHalf;
    float yCenter = sOffsetY + y + hHalf;
    
    float w1 = wHalf;
    float h1 = 0;
    for (float angle = step; angle < circle; angle += step) {
        float w2 = cosf(angle) * wHalf;
        float h2 = sinf(angle) * hHalf;
        
        MWindowSelectPoint0(xCenter - w1, yCenter - h1);
        MWindowSelectPoint1(xCenter - w2, yCenter - h2);
        MWindowSelectPoint2(xCenter     , yCenter     );
        MWindowDrawTriangle();
        
        w1 = w2;
        h1 = h2;
    }
    
    //floating-point operations are biased. make sure to draw last triangle.
    MWindowSelectPoint0(xCenter - w1   , yCenter - h1);
    MWindowSelectPoint1(xCenter - wHalf, yCenter     );
    MWindowSelectPoint2(xCenter        , yCenter     );
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
