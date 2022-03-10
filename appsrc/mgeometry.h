#pragma once

#include "mobject.h"

//point:
//

declare_reflectable_class(MPoint)
class MPoint : public MExtends<MPoint, MObject> {
public:
    MPoint();
    MPoint(float x, float y);

    void setX(float x);
    void setY(float y);

    float x();
    float y();

private:
    float mX;
    float mY;
};

//size:
//

declare_reflectable_class(MSize)
class MSize : public MExtends<MSize, MObject> {
public:
    MSize();
    MSize(float width, float height);

    void setWidth (float width );
    void setHeight(float height);

    float width ();
    float height();

private:
    float mWidth ;
    float mHeight;
};

//rect:
//

declare_reflectable_class(MRect)
class MRect : public MExtends<MRect, MObject> {
public:
    MRect();
    MRect(const MPoint::ptr &origin, const MSize::ptr &size);
    MRect(float x, float y, float width, float height);

    void setOrigin(const MPoint::ptr &origin);
    void setSize  (const MSize ::ptr &size  );

    const MPoint::ptr &origin();
    const MSize ::ptr &size  ();

private:
    MPoint::ptr mOrigin;
    MSize ::ptr mSize  ;
};
