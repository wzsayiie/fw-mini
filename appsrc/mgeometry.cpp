#include "mgeometry.h"
#include <algorithm>

//point:
//

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

define_reflectable_class_function(MPoint, copy)
MPoint::ptr MPoint::copy() {
    return MPoint::create(mX, mY);
}

//size:
//

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

define_reflectable_class_function(MSize, none)
bool MSize::none() {
    return mWidth <= 0 && mHeight <= 0;
}

define_reflectable_class_function(MSize, copy)
MSize::ptr MSize::copy() {
    return MSize::create(mWidth, mHeight);
}

//rect:
//

MRect::MRect(float x, float y, float width, float height) {
    mX      = x     ;
    mY      = y     ;
    mWidth  = width ;
    mHeight = height;
}

define_reflectable_class_function(MRect, setX, "args:x;")
void MRect::setX(float x) {
    mX = x;
}

define_reflectable_class_function(MRect, setY, "args:y;")
void MRect::setY(float y) {
    mY = y;
}

define_reflectable_class_function(MRect, setWidth, "args:width;")
void MRect::setWidth(float width) {
    mWidth = width;
}

define_reflectable_class_function(MRect, setHeight, "args:height;")
void MRect::setHeight(float height) {
    mHeight = height;
}

define_reflectable_class_function(MRect, x)
float MRect::x() {
    return mX;
}

define_reflectable_class_function(MRect, y)
float MRect::y() {
    return mY;
}

define_reflectable_class_function(MRect, width)
float MRect::width() {
    return mWidth;
}

define_reflectable_class_function(MRect, height)
float MRect::height() {
    return mHeight;
}

define_reflectable_class_function(MRect, none)
bool MRect::none() {
    return mWidth <= 0 || mHeight <= 0;
}

define_reflectable_class_function(MRect, intersect)
MRect::ptr MRect::intersect(const MRect::ptr &rect) {
    float left   = std::max(mX, rect->mX);
    float top    = std::max(mY, rect->mY);
    float right  = std::min(mX + mWidth , rect->mX + rect->mWidth );
    float bottom = std::min(mY + mHeight, rect->mY + rect->mHeight);

    if (left < right || top < bottom) {
        return MRect::create(left, top, right - left, bottom - top);
    } else {
        return MRect::create(0.f, 0.f, 0.f, 0.f);
    }
}

define_reflectable_class_function(MRect, copy)
MRect::ptr MRect::copy() {
    return MRect::create(mX, mY, mWidth, mHeight);
}
