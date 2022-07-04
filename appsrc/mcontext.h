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

void MContextPushOffset(float x, float y);
void MContextPopOffset ();
void MContextPushClip  (float x, float y, float w, float h);
void MContextPopClip   ();

void MContextMoveToPoint(float x, float y);
void MContextAddToPoint (float x, float y);
void MContextDrawPolygon();

void MContextDrawRect      (float x , float y , float w , float h );
void MContextDrawEllipse   (float x , float y , float w , float h );
void MContextDrawFlatLine  (float x0, float y0, float x1, float y1);
void MContextDrawSquareLine(float x0, float y0, float x1, float y1);
void MContextDrawRoundLine (float x0, float y0, float x1, float y1);
void MContextDrawImage     (float x , float y , float w , float h );
void MContextDrawText      (float x , float y , float w , float h );

//graphs:

m_enum(MGraphType) {
    Clip    = 1,
    Polygon = 2,
    Image   = 3,
    Text    = 4,
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

//polygon graph:

m_class(MPolygonGraph, MGraph) {
public:
    MGraphType type() override;

    int   points();
    float pixelX(int index);
    float pixelY(int index);
    int   rgba  ();

public:
    std::vector<float> mXs;
    std::vector<float> mYs;
    int mRGBA = 0;
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

M_HOST_CALL MVector<MGraph::ptr> *MContextGetGraphs();
M_HOST_CALL void MContextReset();
