#include "mgeometry.h"

//point:
//

MPoint::MPoint() {
    mX = 0;
    mY = 0;
}

MPoint::MPoint(float x, float y) {
    mX = x;
    mY = y;
}

define_reflectable_class_function(MPoint, setX, "args:x;")
void MPoint::setX(float x) {
    mX = x;
}

define_reflectable_class_function(MPoint, setY, "args:y;")
void MPoint::setY(float y) {
    mY = y;
}

define_reflectable_class_function(MPoint, x)
float MPoint::x() {
    return mX;
}

define_reflectable_class_function(MPoint, y)
float MPoint::y() {
    return mY;
}

//size:
//

MSize::MSize() {
    mWidth  = 0;
    mHeight = 0;
}

MSize::MSize(float width, float height) {
    mWidth  = width ;
    mHeight = height;
}

define_reflectable_class_function(MSize, setWidth, "args:width;")
void MSize::setWidth(float width) {
    mWidth = width;
}

define_reflectable_class_function(MSize, setHeight, "args:height;")
void MSize::setHeight(float height) {
    mHeight = height;
}

define_reflectable_class_function(MSize, width)
float MSize::width() {
    return mWidth;
}

define_reflectable_class_function(MSize, height)
float MSize::height() {
    return mHeight;
}

//rect:
//

MRect::MRect() {
    mOrigin = MPoint::create();
    mSize   = MSize ::create();
}

MRect::MRect(const MPoint::ptr &origin, const MSize::ptr &size) {
    mOrigin = MPoint::create();
    mSize   = MSize ::create();

    if (origin) {
        mOrigin->setX(origin->x());
        mOrigin->setY(origin->y());
    }

    if (size) {
        mSize->setWidth (size->width ());
        mSize->setHeight(size->height());
    }
}

MRect::MRect(float x, float y, float width, float height) {
    mOrigin = MPoint::create();
    mSize   = MSize ::create();

    mOrigin->setX(x);
    mOrigin->setY(y);

    mSize->setWidth (width );
    mSize->setHeight(height);
}

define_reflectable_class_function(MRect, setOrigin, "args:origin;")
void MRect::setOrigin(const MPoint::ptr &origin) {
    if (origin) {
        mOrigin->setX(origin->x());
        mOrigin->setY(origin->y());
    } else {
        mOrigin->setX(0);
        mOrigin->setY(0);
    }
}

define_reflectable_class_function(MRect, setSize, "args:size;")
void MRect::setSize(const MSize::ptr &size) {
    if (size) {
        mSize->setWidth (size->width ());
        mSize->setHeight(size->height());
    } else {
        mSize->setWidth (0);
        mSize->setHeight(0);
    }
}

define_reflectable_class_function(MRect, origin)
const MPoint::ptr &MRect::origin() {
    return mOrigin;
}

define_reflectable_class_function(MRect, size)
const MSize::ptr &MRect::size() {
    return mSize;
}
