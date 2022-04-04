#pragma once

#include "mobject.h"

//point:
//

declare_reflectable_class(MPoint)
class MPoint : public MExtends<MPoint, MObject> {
public:
    static MPoint::ptr from(float x, float y);

public:
    float x();
    float y();

private:
    float mX = 0;
    float mY = 0;
};

//size:
//

declare_reflectable_class(MSize)
class MSize : public MExtends<MSize, MObject> {
public:
    static MSize::ptr from(float width, float height);

public:
    float width ();
    float height();
    bool  none  ();

private:
    float mWidth  = 0;
    float mHeight = 0;
};

//rect:
//

declare_reflectable_class(MRect)
class MRect : public MExtends<MRect, MObject> {
public:
    static MRect::ptr from(float x, float y, float width, float height);

public:
    float       x     ();
    float       y     ();
    float       width ();
    float       height();
    MPoint::ptr origin();
    MSize::ptr  size  ();
    bool        none  ();

    MRect::ptr intersects(const MRect::ptr  &rect );
    bool       contains  (const MPoint::ptr &point);

private:
    MPoint::ptr mOrigin;
    MSize::ptr  mSize  ;
};
