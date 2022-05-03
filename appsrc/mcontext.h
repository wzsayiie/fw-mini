#pragma once

#include "mimage.h"

//draw function:

void MContextSelectImage(const MImage::ptr &image);
void MContextSelectText (const std::string &text );

void MContextSelectRGBA     (int     rgba );
void MContextSelectLineWidth(float   width);
void MContextSelectFontSize (float   size );
void MContextSelectHAlign   (MHAlign align);
void MContextSelectVAlign   (MVAlign align);

void MContextPushClip  (float x, float y, float w, float h);
void MContextPopClip   ();
void MContextPushOffset(float x, float y);
void MContextPopOffset ();

void MContextDrawTriangle  (float x0, float y0, float x1, float y1, float x2, float y2);
void MContextDrawRect      (float x , float y , float w , float h );
void MContextDrawEllipse   (float x , float y , float w , float h );
void MContextDrawFlatLine  (float x0, float y0, float x1, float y1);
void MContextDrawSquareLine(float x0, float y0, float x1, float y1);
void MContextDrawRoundLine (float x0, float y0, float x1, float y1);
void MContextDrawImage     (float x , float y , float w , float h );
void MContextDrawText      (float x , float y , float w , float h );

//graphs:

declare_reflectable_enum(MGraphType)
enum class MGraphType {
    Clip     = 1,
    Triangle = 2,
    Image    = 3,
    Text     = 4,
};

m_class(MGraph, MObject) {
public:
    virtual MGraphType type() = 0;
};

//clip graph:

m_class(MClipGraph, MGraph) {
public:
    MGraphType type() override;

    float pixelX();
    float pixelY();
    float pixelW();
    float pixelH();

public:
    float mX = 0;
    float mY = 0;
    float mW = 0;
    float mH = 0;
};

//triangle graph:

m_class(MTriangleGraph, MGraph) {
public:
    MGraphType type() override;

    int   rgba   ();
    float pixelX0();
    float pixelY0();
    float pixelX1();
    float pixelY1();
    float pixelX2();
    float pixelY2();

public:
    int   mRGBA = 0;
    float mX0   = 0;
    float mY0   = 0;
    float mX1   = 0;
    float mY1   = 0;
    float mX2   = 0;
    float mY2   = 0;
};

//image graph:

m_class(MImageGraph, MGraph) {
public:
    MGraphType type() override;

    MImage::ptr image();

    float pixelX();
    float pixelY();
    float pixelW();
    float pixelH();

public:
    MImage::ptr mImage;

    float mX = 0;
    float mY = 0;
    float mW = 0;
    float mH = 0;
};

//text graph:

m_class(MTextGraph, MGraph) {
public:
    MGraphType type() override;

    std::string text();
    float pixelFontSize();

    int     rgba  ();
    MHAlign hAlign();
    MVAlign vAlign();
    float   pixelX();
    float   pixelY();
    float   pixelW();
    float   pixelH();

public:
    std::string mText;

    float   mFontSize = 0;
    int     mRGBA     = 0;
    MHAlign mHAlign   = MHAlign::Center;
    MVAlign mVAlign   = MVAlign::Middle;
    float   mX        = 0;
    float   mY        = 0;
    float   mW        = 0;
    float   mH        = 0;
};

//host functions:

declare_reflectable_special(MVector<MGraph::ptr>)

M_HOST_CALL_FUNCTION MVector<MGraph::ptr> *MContextGetGraphs();
M_HOST_CALL_FUNCTION void MContextReset();
