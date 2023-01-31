#pragma once

#include "mgeneric.h"
#include "mgraphics.h"
#include "mimage.h"

//draw function:

d_exportable void MContextSelectImage(const MImage::ptr &image);
d_exportable void MContextSelectText (const std::string &text );

d_exportable void MContextSelectRGBA     (int     rgba );
d_exportable void MContextSelectLineWidth(float   width);
d_exportable void MContextSelectFontSize (float   size );
d_exportable void MContextSelectHAlign   (MHAlign align);
d_exportable void MContextSelectVAlign   (MVAlign align);

d_exportable void MContextPushOffset(float x, float y);
d_exportable void MContextPopOffset ();
d_exportable void MContextPushClip  (float x, float y, float w, float h);
d_exportable void MContextPopClip   ();

d_exportable void MContextMoveToPoint(float x, float y);
d_exportable void MContextAddToPoint (float x, float y);
d_exportable void MContextDrawPolygon();

d_exportable void MContextDrawRect      (float x , float y , float w , float h );
d_exportable void MContextDrawEllipse   (float x , float y , float w , float h );
d_exportable void MContextDrawFlatLine  (float x0, float y0, float x1, float y1);
d_exportable void MContextDrawSquareLine(float x0, float y0, float x1, float y1);
d_exportable void MContextDrawRoundLine (float x0, float y0, float x1, float y1);
d_exportable void MContextDrawImage     (float x , float y , float w , float h );
d_exportable void MContextDrawText      (float x , float y , float w , float h );

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
    float alpha();

    float pixelX();
    float pixelY();
    float pixelW();
    float pixelH();

public:
    MImage::ptr mImage;

    float mAlpha = 1;
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

M_HOST_CALL d_exportable MVector<MGraph::ptr> *MContextGetGraphs();
M_HOST_CALL d_exportable void MContextReset();
