#include "mcontext.h"

//context data:

static dash::lazy<MVector<MGraph::ptr>> sGraphs;

declare_reflectable_special(MVector<MRect ::ptr>)
declare_reflectable_special(MVector<MPoint::ptr>)

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

define_reflectable_function(MContextPushClip, "args:x,y,w,h")
void MContextPushClip(float x, float y, float w, float h) {
    //push the clip:
    auto clip = MRect::from(x, y, w, h);

    if (!sClipStack->vector.empty()) {
        auto last = sClipStack->vector.back();
        clip = clip->intersects(last);
    }

    sClipStack->vector.push_back(clip);

    //create the graph:
    auto graph = MClipGraph::create();
    graph->mX = clip->x();
    graph->mY = clip->y();
    graph->mW = clip->width ();
    graph->mH = clip->height();

    sGraphs->vector.push_back(graph);
}

define_reflectable_function(MContextPopClip)
void MContextPopClip() {
    if (sClipStack->vector.empty()) {
        return;
    }

    //pop the clip.
    MRect::ptr clip = sClipStack->vector.back();
    sClipStack->vector.pop_back();

    //create the graph:
    auto graph = MClipGraph::create();
    graph->mX = clip->x();
    graph->mY = clip->y();
    graph->mW = clip->width ();
    graph->mH = clip->height();

    sGraphs->vector.push_back(graph);
}

define_reflectable_function(MContextPushOffset, "args:x,y")
void MContextPushOffset(float x, float y) {
    auto current = MPoint::from(sOffsetX, sOffsetY);
    sOffsetStack->vector.push_back(current);
    
    sOffsetX += x;
    sOffsetY += y;
}

define_reflectable_function(MContextPopOffset)
void MContextPopOffset() {
    auto last = MPoint::zero();

    if (!sOffsetStack->vector.empty()) {
        last = sOffsetStack->vector.back();
        sOffsetStack->vector.pop_back();
    }

    sOffsetX = last->x();
    sOffsetY = last->y();
}

define_reflectable_function(MContextDrawTriangle, "args:x0,y0,x1,y1,x2,y2")
void MContextDrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    if (sRGBA.alpha == 0) {
        return;
    }

    if (x0 == x1 && y0 == y1) { return; }
    if (x0 == x2 && y0 == y2) { return; }
    if (x1 == x2 && y1 == y2) { return; }

    auto graph = MTriangleGraph::create();
    graph->mRGBA = sRGBA.rgba;
    graph->mX0   = sOffsetX + x0;
    graph->mY0   = sOffsetY + y0;
    graph->mX1   = sOffsetX + x1;
    graph->mY1   = sOffsetY + y1;
    graph->mX2   = sOffsetX + x2;
    graph->mY2   = sOffsetY + y2;

    sGraphs->vector.push_back(graph);
}

define_reflectable_function(MContextDrawRect, "args:x,y,w,h")
void MContextDrawRect(float x, float y, float w, float h) {
    if (sRGBA.rgba == 0) {
        return;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    //  a - b
    //  |  /|
    //  | / |
    //  |/  |
    //  c - d
    float ax = sOffsetX + x;
    float ay = sOffsetY + y;
    float bx = sOffsetX + x + w;
    float by = sOffsetY + y;
    float cx = sOffsetX + x;
    float cy = sOffsetY + y + h;
    float dx = sOffsetX + x + w;
    float dy = sOffsetY + y + h;

    auto abc = MTriangleGraph::create();
    auto bcd = MTriangleGraph::create();
    abc->mRGBA = sRGBA.rgba;
    bcd->mRGBA = sRGBA.rgba;

    abc->mX0 = ax;
    abc->mY0 = ay;
    abc->mX1 = bx;
    abc->mY1 = by;
    abc->mX2 = cx;
    abc->mY2 = cy;

    bcd->mX0 = bx;
    bcd->mY0 = by;
    bcd->mX1 = cx;
    bcd->mY1 = cy;
    bcd->mX2 = dx;
    bcd->mY2 = dy;

    sGraphs->vector.push_back(abc);
    sGraphs->vector.push_back(bcd);
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

    sGraphs->vector.push_back(graph);
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

    sGraphs->vector.push_back(graph);
}

//graphs:

define_reflectable_enum_const(MGraphType, Clip    )
define_reflectable_enum_const(MGraphType, Triangle)
define_reflectable_enum_const(MGraphType, Image   )
define_reflectable_enum_const(MGraphType, Text    )

//clip graph:

define_reflectable_class_function(MClipGraph, type)
MGraphType MClipGraph::type() {
    implement_injectable_function((MGraphType)(int))
    return MGraphType::Clip;
}

define_reflectable_class_function(MClipGraph, pixelX)
define_reflectable_class_function(MClipGraph, pixelY)
define_reflectable_class_function(MClipGraph, pixelW)
define_reflectable_class_function(MClipGraph, pixelH)

float MClipGraph::pixelX() { return m_px_from_pt mX; }
float MClipGraph::pixelY() { return m_px_from_pt mY; }
float MClipGraph::pixelW() { return m_px_from_pt mW; }
float MClipGraph::pixelH() { return m_px_from_pt mH; }

//triangle graph:

define_reflectable_class_function(MTriangleGraph, type)
MGraphType MTriangleGraph::type() {
    implement_injectable_function((MGraphType)(int))
    return MGraphType::Triangle;
}

define_reflectable_class_function(MTriangleGraph, rgba   )
define_reflectable_class_function(MTriangleGraph, pixelX0)
define_reflectable_class_function(MTriangleGraph, pixelY0)
define_reflectable_class_function(MTriangleGraph, pixelX1)
define_reflectable_class_function(MTriangleGraph, pixelY1)
define_reflectable_class_function(MTriangleGraph, pixelX2)
define_reflectable_class_function(MTriangleGraph, pixelY2)

int   MTriangleGraph::rgba   () { return mRGBA           ; }
float MTriangleGraph::pixelX0() { return m_px_from_pt mX0; }
float MTriangleGraph::pixelY0() { return m_px_from_pt mY0; }
float MTriangleGraph::pixelX1() { return m_px_from_pt mX1; }
float MTriangleGraph::pixelY1() { return m_px_from_pt mY1; }
float MTriangleGraph::pixelX2() { return m_px_from_pt mX2; }
float MTriangleGraph::pixelY2() { return m_px_from_pt mY2; }

//image graph:

define_reflectable_class_function(MImageGraph, type)
MGraphType MImageGraph::type() {
    implement_injectable_function((MGraphType)(int))
    return MGraphType::Image;
}

define_reflectable_class_function(MImageGraph, image)
MImage::ptr MImageGraph::image() {
    return mImage;
}

define_reflectable_class_function(MImageGraph, pixelX)
define_reflectable_class_function(MImageGraph, pixelY)
define_reflectable_class_function(MImageGraph, pixelW)
define_reflectable_class_function(MImageGraph, pixelH)

float MImageGraph::pixelX() { return m_px_from_pt mX; }
float MImageGraph::pixelY() { return m_px_from_pt mY; }
float MImageGraph::pixelW() { return m_px_from_pt mW; }
float MImageGraph::pixelH() { return m_px_from_pt mH; }

//text graph:

define_reflectable_class_function(MTextGraph, type)
MGraphType MTextGraph::type() {
    implement_injectable_function((MGraphType)(int))
    return MGraphType::Text;
}

define_reflectable_class_function(MTextGraph, text)
std::string MTextGraph::text() {
    return mText;
}

define_reflectable_class_function(MTextGraph, pixelFontSize)
float MTextGraph::pixelFontSize() {
    return m_px_from_pt mFontSize;
}

define_reflectable_class_function(MTextGraph, rgba  )
define_reflectable_class_function(MTextGraph, hAlign)
define_reflectable_class_function(MTextGraph, vAlign)

int     MTextGraph::rgba  () { return mRGBA  ; }
MHAlign MTextGraph::hAlign() { return mHAlign; }
MVAlign MTextGraph::vAlign() { return mVAlign; }

define_reflectable_class_function(MTextGraph, pixelX)
define_reflectable_class_function(MTextGraph, pixelY)
define_reflectable_class_function(MTextGraph, pixelW)
define_reflectable_class_function(MTextGraph, pixelH)

float MTextGraph::pixelX() { return m_px_from_pt mX; }
float MTextGraph::pixelY() { return m_px_from_pt mY; }
float MTextGraph::pixelW() { return m_px_from_pt mW; }
float MTextGraph::pixelH() { return m_px_from_pt mH; }

//host functions:

MVector<MGraph::ptr> *MContextGetGraphs() {
    return &sGraphs;
}

void MContextReset() {
    sGraphs->vector.clear();

    sImage = nullptr;
    sText  = "";
}
