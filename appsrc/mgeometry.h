#pragma once

#include "mobject.h"

//point:
//

declare_reflectable_class(MPoint)
class MPoint : public MExtends<MPoint, MObject> {
public:
    MPoint() = default;
    MPoint(float x, float y);

    void setX(float x);
    void setY(float y);

    float x();
    float y();

    MPoint::ptr copy();

private:
    float mX = 0;
    float mY = 0;
};

//size:
//

declare_reflectable_class(MSize)
class MSize : public MExtends<MSize, MObject> {
public:
    MSize() = default;
    MSize(float width, float height);

    void setWidth (float width );
    void setHeight(float height);

    float width ();
    float height();

    MSize::ptr copy();

private:
    float mWidth  = 0;
    float mHeight = 0;
};

//rect:
//

declare_reflectable_class(MRect)
class MRect : public MExtends<MRect, MObject> {
public:
    MRect() = default;
    MRect(float x, float y, float width, float height);

    void setX     (float x     );
    void setY     (float y     );
    void setWidth (float width );
    void setHeight(float height);

    float x     ();
    float y     ();
    float width ();
    float height();

    MRect::ptr copy();

private:
    float mX      = 0;
    float mY      = 0;
    float mWidth  = 0;
    float mHeight = 0;
};
