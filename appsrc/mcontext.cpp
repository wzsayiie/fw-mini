#include "mcontext.h"

//context data:

static dash::lazy<MVector<MGraph::ptr>> sGraphs;

static dash::lazy<MVector<MRect ::ptr>> sClipStack;
static dash::lazy<MVector<MPoint::ptr>> sOffsetStack;
static float sOffsetX = 0;
static float sOffsetY = 0;

static dash::lazy<MImage::ptr> sImage;
static dash::lazy<std::string> sText;

static MColorRGBA sRGBA      = {0};
static float      sLineWidth = 0;
static float      sFontSize  = 0;
static MHAlign    sHAlign    = MHAlign::Center;
static MVAlign    sVAlign    = MVAlign::Middle;

static dash::lazy<MPolygonGraph::ptr> sPolygon;

//draw function:

define_reflectable_function(MContextSelectImage    , "args:image")
define_reflectable_function(MContextSelectText     , "args:text" )
define_reflectable_function(MContextSelectRGBA     , "args:rgba" )
define_reflectable_function(MContextSelectLineWidth, "args:width")
define_reflectable_function(MContextSelectFontSize , "args:size" )
define_reflectable_function(MContextSelectHAlign   , "args:align")
define_reflectable_function(MContextSelectVAlign   , "args:align")

void MContextSelectImage(const MImage::ptr &image) { sImage = image; }
void MContextSelectText (const std::string &text ) { sText  = text ; }

void MContextSelectRGBA     (int     rgba ) { sRGBA.rgba = rgba ; }
void MContextSelectLineWidth(float   width) { sLineWidth = width; }
void MContextSelectFontSize (float   size ) { sFontSize  = size ; }
void MContextSelectHAlign   (MHAlign align) { sHAlign    = align; }
void MContextSelectVAlign   (MVAlign align) { sVAlign    = align; }

define_reflectable_function(MContextPushOffset, "args:x,y")
void MContextPushOffset(float x, float y) {
    auto current = MPoint::from(sOffsetX, sOffsetY);
    sOffsetStack->push_back(current);
    
    sOffsetX += x;
    sOffsetY += y;
}

define_reflectable_function(MContextPopOffset)
void MContextPopOffset() {
    auto last = MPoint::zero();

    if (!sOffsetStack->empty()) {
        last = sOffsetStack->back();
        sOffsetStack->pop_back();
    }

    sOffsetX = last->x();
    sOffsetY = last->y();
}

define_reflectable_function(MContextPushClip, "args:x,y,w,h")
void MContextPushClip(float x, float y, float w, float h) {
    //push the clip:
    auto clip = MRect::from(sOffsetX + x, sOffsetY + y, w, h);
    if (!sClipStack->empty()) {
        MRect::ptr last = sClipStack->back();
        clip = clip->intersects(last);
    }

    sClipStack->push_back(clip);

    //create the graph:
    auto graph = MClipGraph::create();
    graph->mX = clip->x();
    graph->mY = clip->y();
    graph->mW = clip->width ();
    graph->mH = clip->height();

    sGraphs->push_back(graph);
}

define_reflectable_function(MContextPopClip)
void MContextPopClip() {
    if (sClipStack->empty()) {
        return;
    }

    //pop the clip.
    MRect::ptr clip = sClipStack->back();
    sClipStack->pop_back();

    //create the graph:
    auto graph = MClipGraph::create();
    graph->mX = clip->x();
    graph->mY = clip->y();
    graph->mW = clip->width ();
    graph->mH = clip->height();

    sGraphs->push_back(graph);
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
    if (sRGBA.rgba == 0) {
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
    //TODO.
}

define_reflectable_function(MContextDrawFlatLine, "args:x0,y0,x1,y1")
void MContextDrawFlatLine(float x0, float y0, float x1, float y1) {
    //TODO.
}

define_reflectable_function(MContextDrawSquareLine, "args:x0,y0,x1,y1")
void MContextDrawSquareLine(float x0, float y0, float x1, float y1) {
    //TODO.
}

define_reflectable_function(MContextDrawRoundLine, "args:x0,y0,x1,y1")
void MContextDrawRoundLine(float x0, float y0, float x1, float y1) {
    //TODO.
}

define_reflectable_function(MContextDrawImage, "args:x,y,w,h")
void MContextDrawImage(float x, float y, float w, float h) {
    if (sImage == nullptr) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    auto graph = MImageGraph::create();
    graph->mImage = sImage;
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
