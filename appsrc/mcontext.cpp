#define _USE_MATH_DEFINES

#include "mcontext.h"
#include <algorithm>
#include <cmath>
#include "dlazy.h"
#include "rdefine.h"

//context data:

static dash::lazy<MVector<MGraph::ptr>> sGraphs;

static float sOffsetX = 0;
static float sOffsetY = 0;
static float sAlpha   = 1;

static dash::lazy<MImage::ptr> sImage;
static dash::lazy<std::string> sText;

static MColorRGBA sRGBA      = {0};
static float      sLineWidth = 0;
static float      sFontSize  = 0;
static MHAlign    sHAlign    = MHAlign::Center;
static MVAlign    sVAlign    = MVAlign::Middle;

static dash::lazy<MPolygonGraph::ptr> sPolygon;

//draw function:

define_reflectable_function(MContextSetOffset, "args:x,y")
void MContextSetOffset(float x, float y) {
    sOffsetX = x;
    sOffsetY = y;
}

define_reflectable_function(MContextSetClip, "args:x,y,w,h")
void MContextSetClip(float x, float y, float w, float h) {
    auto graph = MClipGraph::create();
    graph->mX = x;
    graph->mY = y;
    graph->mW = w;
    graph->mH = h;

    sGraphs->push_back(graph);
}

define_reflectable_function(MContextSetAlpha, "args:alpha")
void MContextSetAlpha(float alpha) {
    if (alpha < 0) {
        alpha = 0;
    } else if (alpha > 1) {
        alpha = 1;
    }
    
    sAlpha = alpha;
}

define_reflectable_function(MContextSelectImage    , "args:image")
define_reflectable_function(MContextSelectText     , "args:text" )
define_reflectable_function(MContextSelectLineWidth, "args:width")
define_reflectable_function(MContextSelectFontSize , "args:size" )
define_reflectable_function(MContextSelectHAlign   , "args:align")
define_reflectable_function(MContextSelectVAlign   , "args:align")

void MContextSelectImage(const MImage::ptr &image) { sImage = image; }
void MContextSelectText (const std::string &text ) { sText  = text ; }

void MContextSelectLineWidth(float   width) { sLineWidth = width; }
void MContextSelectFontSize (float   size ) { sFontSize  = size ; }
void MContextSelectHAlign   (MHAlign align) { sHAlign    = align; }
void MContextSelectVAlign   (MVAlign align) { sVAlign    = align; }

define_reflectable_function(MContextSelectRGBA, "args:rgba")
void MContextSelectRGBA(int rgba) {
    sRGBA.rgba = rgba;
    sRGBA.alpha *= sAlpha;
}

define_reflectable_function(MContextMoveToPoint, "args:x,y")
void MContextMoveToPoint(float x, float y) {
    sPolygon = MPolygonGraph::create();
    sPolygon->mXs.push_back(sOffsetX + x);
    sPolygon->mYs.push_back(sOffsetY + y);
}

define_reflectable_function(MContextAddToPoint, "args:x,y")
void MContextAddToPoint(float x, float y) {
    if (sPolygon) {
        sPolygon->mXs.push_back(sOffsetX + x);
        sPolygon->mYs.push_back(sOffsetY + y);
    }
}

define_reflectable_function(MContextDrawPolygon)
void MContextDrawPolygon() {
    if (!sPolygon) {
        return;
    }
    
    if (sPolygon->points() >= 3 && sRGBA.alpha != 0) {
        sPolygon->mRGBA = sRGBA.rgba;
        sGraphs->push_back((MPolygonGraph::ptr)sPolygon);
    }
    sPolygon = nullptr;
}

define_reflectable_function(MContextDrawRect, "args:x,y,w,h")
void MContextDrawRect(float x, float y, float w, float h) {
    if (sRGBA.alpha == 0) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }
    
    MContextMoveToPoint(x    , y    );
    MContextAddToPoint (x + w, y    );
    MContextAddToPoint (x + w, y + h);
    MContextAddToPoint (x    , y + h);
    MContextDrawPolygon();
}

define_reflectable_function(MContextDrawEllipse, "args:x,y,w,h")
void MContextDrawEllipse(float x, float y, float w, float h) {
    if (sRGBA.alpha == 0) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    //if the ellipse is small, draw a rectangle.
    if (w <= 2 || h <= 2) {
        MContextDrawRect(x, y, w, h);
        return;
    }

    int   vnum = std::clamp((int)fminf(w, h) / 2, 4, 64);
    float step = (float)M_PI * 2.f / vnum;
    float wsem = w / 2.f;
    float hsem = h / 2.f;

    for (int i = 0; i < vnum; ++i) {
        float xpt = x + wsem + wsem * cosf(step * i);
        float ypt = y + hsem + hsem * sinf(step * i);

        if (i == 0) {
            MContextMoveToPoint(xpt, ypt);
        } else {
            MContextAddToPoint (xpt, ypt);
        }
    }
    MContextDrawPolygon();
}

define_reflectable_function(MContextDrawFlatLine, "args:x0,y0,x1,y1")
void MContextDrawFlatLine(float x0, float y0, float x1, float y1) {
    if (sRGBA.alpha == 0) {
        return;
    }
    if (sLineWidth <= 0) {
        return;
    }
    if (x0 == x1 && y0 == y1) {
        return;
    }

    float sem = sLineWidth / 2.f;

    //a vertical line.
    if (x0 == x1) {
        MContextDrawRect(
            x0 - sem  , fminf(y0,  y1),
            sLineWidth, fabsf(y0 - y1)
        );
        return;
    }

    //a slash line:
    float slope = atanf((y0 - y1) / (x0 - x1));
    float pi_2  = (float)M_PI_2;

    MContextMoveToPoint(x0 + sem * cosf(slope + pi_2  ), y0 + sem * sinf(slope + pi_2  ));
    MContextAddToPoint (x0 + sem * cosf(slope + pi_2*3), y0 + sem * sinf(slope + pi_2*3));
    MContextAddToPoint (x1 + sem * cosf(slope + pi_2*3), y1 + sem * sinf(slope + pi_2*3));
    MContextAddToPoint (x1 + sem * cosf(slope + pi_2  ), y1 + sem * sinf(slope + pi_2  ));
    MContextDrawPolygon();
}

define_reflectable_function(MContextDrawSquareLine, "args:x0,y0,x1,y1")
void MContextDrawSquareLine(float x0, float y0, float x1, float y1) {
    if (sRGBA.alpha == 0) {
        return;
    }
    if (sLineWidth <= 0) {
        return;
    }

    float sem = sLineWidth / 2.f;

    //if the start point is the same as the end point, draw a square.
    if (x0 == x1 && y0 == y1) {
        MContextDrawRect(x0 - sem, y0 - sem, sLineWidth, sLineWidth);
        return;
    }

    //a vertical line.
    if (x0 == x1) {
        MContextDrawFlatLine(
            x0, fminf(y0, y1) - sem,
            x0, fmaxf(y0, y1) + sem
        );
        return;
    }

    //a slash line:
    float slope = atanf((y0 - y1) / (x0 - x1));
    float cosv  = fabsf(sem * cosf(slope));
    float sinv  = fabsf(sem * sinf(slope));

    MContextDrawFlatLine(
        x0 + (x0 < x1 ? -1.f : 1.f) * cosv, y0 + (y0 < y1 ? -1.f : 1.f) * sinv,
        x1 + (x1 < x0 ? -1.f : 1.f) * cosv, y1 + (y1 < y0 ? -1.f : 1.f) * sinv
    );
}

define_reflectable_function(MContextDrawRoundLine, "args:x0,y0,x1,y1")
void MContextDrawRoundLine(float x0, float y0, float x1, float y1) {
    if (sRGBA.alpha == 0) {
        return;
    }
    if (sLineWidth <= 0) {
        return;
    }

    float sem = sLineWidth / 2.f;

    //if the start point is the same as the end point, draw a circle.
    if (x0 == x1 && y0 == y1) {
        MContextDrawEllipse(x0 - sem, y0 - sem, sLineWidth, sLineWidth);
        return;
    }

    MContextDrawFlatLine(x0, y0, x1, y1);
    MContextDrawEllipse(x0 - sem, y0 - sem, sLineWidth, sLineWidth);
    MContextDrawEllipse(x1 - sem, y1 - sem, sLineWidth, sLineWidth);
}

define_reflectable_function(MContextDrawImage, "args:x,y,w,h")
void MContextDrawImage(float x, float y, float w, float h) {
    if (sImage == nullptr) {
        return;
    }
    if (sRGBA.alpha == 0) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    auto graph = MImageGraph::create();
    graph->mImage = sImage;
    graph->mAlpha = sRGBA.alpha / 255.f;
    graph->mX     = sOffsetX + x;
    graph->mY     = sOffsetY + y;
    graph->mW     = w;
    graph->mH     = h;

    sGraphs->push_back(graph);
}

define_reflectable_function(MContextDrawText, "args:x,y,w,h")
void MContextDrawText(float x, float y, float w, float h) {
    if (sText->empty()) {
        return;
    }
    if (sRGBA.alpha == 0) {
        return;
    }
    if (sFontSize <= 0) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    auto graph = MTextGraph::create();
    graph->mText     = sText;
    graph->mRGBA     = sRGBA.rgba;
    graph->mFontSize = sFontSize;
    graph->mHAlign   = sHAlign;
    graph->mVAlign   = sVAlign;
    graph->mX        = sOffsetX + x;
    graph->mY        = sOffsetY + y;
    graph->mW        = w;
    graph->mH        = h;

    sGraphs->push_back(graph);
}

//graphs:

define_reflectable_enum_const(MGraphType, Clip   )
define_reflectable_enum_const(MGraphType, Polygon)
define_reflectable_enum_const(MGraphType, Image  )
define_reflectable_enum_const(MGraphType, Text   )

//clip graph:

define_reflectable_class_function(MClipGraph, type, "virtual;getter")
MGraphType MClipGraph::type() {
    implement_injectable_function(MGraphType)
    return MGraphType::Clip;
}

define_reflectable_class_function(MClipGraph, pixelX, "getter")
define_reflectable_class_function(MClipGraph, pixelY, "getter")
define_reflectable_class_function(MClipGraph, pixelW, "getter")
define_reflectable_class_function(MClipGraph, pixelH, "getter")

float MClipGraph::pixelX() { return m_px_from_pt mX; }
float MClipGraph::pixelY() { return m_px_from_pt mY; }
float MClipGraph::pixelW() { return m_px_from_pt mW; }
float MClipGraph::pixelH() { return m_px_from_pt mH; }

//polygon graph:

define_reflectable_class_function(MPolygonGraph, type, "virtual;getter")
MGraphType MPolygonGraph::type() {
    implement_injectable_function(MGraphType)
    return MGraphType::Polygon;
}

define_reflectable_class_function(MPolygonGraph, points, "getter"  )
define_reflectable_class_function(MPolygonGraph, pixelX, "args:index")
define_reflectable_class_function(MPolygonGraph, pixelY, "args:index")
define_reflectable_class_function(MPolygonGraph, rgba  , "getter"  )

int   MPolygonGraph::points()      { return (int)mXs.size()    ; }
float MPolygonGraph::pixelX(int i) { return m_px_from_pt mXs[i]; }
float MPolygonGraph::pixelY(int i) { return m_px_from_pt mYs[i]; }
int   MPolygonGraph::rgba  ()      { return mRGBA              ; }

//image graph:

define_reflectable_class_function(MImageGraph, type, "virtual;getter")
MGraphType MImageGraph::type() {
    implement_injectable_function(MGraphType)
    return MGraphType::Image;
}

define_reflectable_class_function(MImageGraph, image, "getter")
MImage::ptr MImageGraph::image() {
    return mImage;
}

define_reflectable_class_function(MImageGraph, alpha, "getter")
float MImageGraph::alpha() {
    return mAlpha;
}

define_reflectable_class_function(MImageGraph, pixelX, "getter")
define_reflectable_class_function(MImageGraph, pixelY, "getter")
define_reflectable_class_function(MImageGraph, pixelW, "getter")
define_reflectable_class_function(MImageGraph, pixelH, "getter")

float MImageGraph::pixelX() { return m_px_from_pt mX; }
float MImageGraph::pixelY() { return m_px_from_pt mY; }
float MImageGraph::pixelW() { return m_px_from_pt mW; }
float MImageGraph::pixelH() { return m_px_from_pt mH; }

//text graph:

define_reflectable_class_function(MTextGraph, type, "virtual;getter")
MGraphType MTextGraph::type() {
    implement_injectable_function(MGraphType)
    return MGraphType::Text;
}

define_reflectable_class_function(MTextGraph, text, "getter")
std::string MTextGraph::text() {
    return mText;
}

define_reflectable_class_function(MTextGraph, pixelFontSize, "getter")
float MTextGraph::pixelFontSize() {
    return m_px_from_pt mFontSize;
}

define_reflectable_class_function(MTextGraph, rgba  , "getter")
define_reflectable_class_function(MTextGraph, hAlign, "getter")
define_reflectable_class_function(MTextGraph, vAlign, "getter")

int     MTextGraph::rgba  () { return mRGBA  ; }
MHAlign MTextGraph::hAlign() { return mHAlign; }
MVAlign MTextGraph::vAlign() { return mVAlign; }

define_reflectable_class_function(MTextGraph, pixelX, "getter")
define_reflectable_class_function(MTextGraph, pixelY, "getter")
define_reflectable_class_function(MTextGraph, pixelW, "getter")
define_reflectable_class_function(MTextGraph, pixelH, "getter")

float MTextGraph::pixelX() { return m_px_from_pt mX; }
float MTextGraph::pixelY() { return m_px_from_pt mY; }
float MTextGraph::pixelW() { return m_px_from_pt mW; }
float MTextGraph::pixelH() { return m_px_from_pt mH; }

//host functions:

MVector<MGraph::ptr> *MContextGetGraphs() {
    return &sGraphs;
}

void MContextReset() {
    sGraphs->clear();

    sImage = nullptr;
    sText  = "";
}
