#define _USE_MATH_DEFINES

#include "mcontext.h"
#include <algorithm>
#include <cmath>

struct Clip {
    float left   = 0;
    float top    = 0;
    float right  = 0;
    float bottom = 0;
};
static std::vector<Clip> sClips;

static float sDrawOffsetX = 0;
static float sDrawOffsetY = 0;
static float sLineWidth   = 0;

void MContextPushClip(float x, float y, float width, float height) {
    Clip clip;
    clip.left   = x;
    clip.top    = y;
    clip.right  = x + width ;
    clip.bottom = y + height;
    
    //intersection with the last clip.
    if (sClips.size() > 0) {
        Clip last = *sClips.rbegin();
        
        clip.left   = std::max(clip.left  , last.left  );
        clip.top    = std::max(clip.top   , last.top   );
        clip.right  = std::min(clip.right , last.right );
        clip.bottom = std::min(clip.bottom, last.bottom);
    }
    sClips.push_back(clip);
    
    MWindowSelectPoint0(clip.left , clip.top   );
    MWindowSelectPoint1(clip.right, clip.bottom);
    MWindowDrawClip();
}

void MContextPopClip() {
    if (sClips.size() > 0) {
        sClips.pop_back();
    }
    
    if (sClips.size() > 0) {
        Clip clip = *sClips.rbegin();
        
        MWindowSelectPoint0(clip.left , clip.top   );
        MWindowSelectPoint1(clip.right, clip.bottom);
        MWindowDrawClip();
    }
}

void MContextSetDrawOffset(float x, float y) {
    sDrawOffsetX = x;
    sDrawOffsetY = y;
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

void MContextDrawTriangle0(float x0, float y0) { MWindowSelectPoint0(sDrawOffsetX + x0, sDrawOffsetY + y0); }
void MContextDrawTriangle1(float x1, float y1) { MWindowSelectPoint1(sDrawOffsetX + x1, sDrawOffsetY + y1); }
void MContextDrawTriangle2(float x2, float y2) { MWindowSelectPoint2(sDrawOffsetX + x2, sDrawOffsetY + y2);
    MWindowDrawTriangle();
}

void MContextDrawFlatLine(float x0, float y0, float x1, float y1) {
}

void MContextDrawSquareLine(float x0, float y0, float x1, float y1) {
}

void MContextDrawRoundLine(float x0, float y0, float x1, float y1) {
}

void MContextDrawRect(float x, float y, float width, float height) {
    float left   = sDrawOffsetX + x;
    float top    = sDrawOffsetY + y;
    float right  = sDrawOffsetX + x + width ;
    float bottom = sDrawOffsetY + y + height;
    
    MWindowSelectPoint0(left , top   );
    MWindowSelectPoint1(right, top   );
    MWindowSelectPoint2(right, bottom);
    MWindowDrawTriangle();
    
    MWindowSelectPoint0(left , top   );
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
    float xCenter = sDrawOffsetX + x + wHalf;
    float yCenter = sDrawOffsetY + y + hHalf;
    
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
    float left   = sDrawOffsetX + x;
    float top    = sDrawOffsetY + y;
    float right  = sDrawOffsetX + x + width ;
    float bottom = sDrawOffsetY + y + height;

    MWindowSelectPoint0(left , top   );
    MWindowSelectPoint1(right, bottom);

    MWindowDrawImage();
}

void MContextDrawString(float x, float y, float width, float height) {
    float left   = sDrawOffsetX + x;
    float top    = sDrawOffsetY + y;
    float right  = sDrawOffsetX + x + width ;
    float bottom = sDrawOffsetY + y + height;

    MWindowSelectPoint0(left , top   );
    MWindowSelectPoint1(right, bottom);

    MWindowDrawLabel();
}
