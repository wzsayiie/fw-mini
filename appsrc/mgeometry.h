#pragma once

#include "mobject.h"

//point:

m_class(MPoint, MObject) {
public:
    static MPoint::ptr from(float x, float y);
    static MPoint::ptr zero();

public:
    float x();
    float y();

    bool equal(const MPoint::ptr &that);

    MPoint::ptr add(const MPoint::ptr &that);
    MPoint::ptr sub(const MPoint::ptr &that);

private:
    float mX = 0;
    float mY = 0;
};

//size:

m_class(MSize, MObject) {
public:
    static MSize::ptr from(float width, float height);
    static MSize::ptr zero();

public:
    float width ();
    float height();
    bool  none  ();

    bool equal(const MSize::ptr &that);

private:
    float mWidth  = 0;
    float mHeight = 0;
};

//rect:

m_class(MRect, MObject) {
public:
    static MRect::ptr from(float x, float y, float width, float height);
    static MRect::ptr zero();

public:
    float       x     ();
    float       y     ();
    float       width ();
    float       height();
    MPoint::ptr origin();
    MSize::ptr  size  ();
    bool        none  ();

    bool equal(const MRect::ptr &that);

    MRect::ptr intersects(const MRect::ptr  &rect );
    bool       contains  (const MPoint::ptr &point);

private:
    MPoint::ptr mOrigin;
    MSize::ptr  mSize  ;
};
