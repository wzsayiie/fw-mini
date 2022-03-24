#include "mcontext.h"

//context data:
//

static dash::lazy_var<MVector<MGraph::ptr>> sGraphs;

static dash::lazy_var<MVector<MPoint>> sOffsetStack;
static dash::lazy_var<MVector<MRect >> sClipStack;
static float sOffsetX = 0;
static float sOffsetY = 0;

static dash::lazy_var<MImage::ptr> sImage;
static dash::lazy_var<std::string> sText;
static float   sFontSize  = 0;
static float   sLineWidth = 0;
static MHAlign sHAlign    = MHAlign::Center;
static MVAlign sVAlign    = MVAlign::Middle;
static int     sRGBA      = 0;

//draw function:
//

define_reflectable_function(MContextSelectImage, "args:image;")
void MContextSelectImage(const MImage::ptr &image) {
}

define_reflectable_function(MContextSelectText, "args:text;")
void MContextSelectText(const std::string &text) {
}

define_reflectable_function(MContextSelectFontSize, "args:size;")
void MContextSelectFontSize(float size) {
}

define_reflectable_function(MContextSelectLineWidth, "args:width;")
void MContextSelectLineWidth(float width) {
}

define_reflectable_function(MContextSelectHAlign, "args:align;")
void MContextSelectHAlign(MHAlign align) {
}

define_reflectable_function(MContextSelectVAlign, "args:align;")
void MContextSelectVAlign(MVAlign align) {
}

define_reflectable_function(MContextSelectRGBA, "args:rgba;")
void MContextSelectRGBA(int rgba) {
}

define_reflectable_function(MContextPushClip, "args:x,y,w,h;")
void MContextPushClip(float x, float y, float w, float h) {
}

define_reflectable_function(MContextPopClip)
void MContextPopClip() {
}

define_reflectable_function(MContextPushOffset, "args:x,y;")
void MContextPushOffset(float x, float y) {
}

define_reflectable_function(MContextPopOffset)
void MContextPopOffset() {
}

define_reflectable_function(MContextDrawTriangle, "args:x0,y0,x1,y1,x2,y2;")
void MContextDrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
}

define_reflectable_function(MContextDrawRect, "args:x,y,w,h;")
void MContextDrawRect(float x, float y, float w, float h) {
}

define_reflectable_function(MContextDrawEllipse, "args:x,y,w,h;")
void MContextDrawEllipse(float x, float y, float w, float h) {
}

define_reflectable_function(MContextDrawFlatLine, "args:x0,y0,x1,y1;")
void MContextDrawFlatLine(float x0, float y0, float x1, float y1) {
}

define_reflectable_function(MContextDrawSquareLine, "args:x0,y0,x1,y1;")
void MContextDrawSquareLine(float x0, float y0, float x1, float y1) {
}

define_reflectable_function(MContextDrawRoundLine, "args:x0,y0,x1,y1;")
void MContextDrawRoundLine(float x0, float y0, float x1, float y1) {
}

define_reflectable_function(MContextDrawImage, "args:x,y,w,h;")
void MContextDrawImage(float x, float y, float w, float h) {
}

define_reflectable_function(MContextDrawString, "args:x,y,w,h;")
void MContextDrawString(float x, float y, float w, float h) {
}

//graphs:
//

define_reflectable_enum_const(MGraphType, Clip    )
define_reflectable_enum_const(MGraphType, Triangle)
define_reflectable_enum_const(MGraphType, Image   )
define_reflectable_enum_const(MGraphType, Text    )

//clip graph:
//

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
//

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
//

define_reflectable_class_function(MImageGraph, type)
MGraphType MImageGraph::type() {
    implement_injectable_function((MGraphType)(int))
    return MGraphType::Image;
}

define_reflectable_class_function(MImageGraph, image )
define_reflectable_class_function(MImageGraph, pixelX)
define_reflectable_class_function(MImageGraph, pixelY)
define_reflectable_class_function(MImageGraph, pixelW)
define_reflectable_class_function(MImageGraph, pixelH)

MImage::ptr MImageGraph::image() { return mImage; }

float MImageGraph::pixelX() { return m_px_from_pt mX; }
float MImageGraph::pixelY() { return m_px_from_pt mY; }
float MImageGraph::pixelW() { return m_px_from_pt mW; }
float MImageGraph::pixelH() { return m_px_from_pt mH; }

//text graph:
//

define_reflectable_class_function(MTextGraph, type)
MGraphType MTextGraph::type() {
    implement_injectable_function((MGraphType)(int))
    return MGraphType::Text;
}

define_reflectable_class_function(MTextGraph, text    )
define_reflectable_class_function(MTextGraph, rgba    )
define_reflectable_class_function(MTextGraph, fontSize)
define_reflectable_class_function(MTextGraph, hAlign  )
define_reflectable_class_function(MTextGraph, vAlign  )
define_reflectable_class_function(MTextGraph, pixelX  )
define_reflectable_class_function(MTextGraph, pixelY  )
define_reflectable_class_function(MTextGraph, pixelW  )
define_reflectable_class_function(MTextGraph, pixelH  )

std::string MTextGraph::text() { return mText; }

int     MTextGraph::rgba    () { return mRGBA    ; }
float   MTextGraph::fontSize() { return mFontSize; }
MHAlign MTextGraph::hAlign  () { return mHAlign  ; }
MVAlign MTextGraph::vAlign  () { return mVAlign  ; }

float MTextGraph::pixelX() { return m_px_from_pt mX; }
float MTextGraph::pixelY() { return m_px_from_pt mY; }
float MTextGraph::pixelW() { return m_px_from_pt mW; }
float MTextGraph::pixelH() { return m_px_from_pt mH; }

//host functions:
//

MVector<MGraph::ptr> *MContextGetGraphs() {
    return &sGraphs;
}

void MContextReset() {
    sGraphs->vector.clear();
}
