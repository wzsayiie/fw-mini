#include "mgeometry.h"

//point:

define_reflectable_class_function(MPoint, from, "args:x,y")
MPoint::ptr MPoint::from(float x, float y) {
    if (x || y) {
        auto obj = MPoint::create();
        obj->mX = x;
        obj->mY = y;

        return obj;

    } else {
        return zero();
    }
}

define_reflectable_class_function(MPoint, zero, "getter")
MPoint::ptr MPoint::zero() {
    static auto obj = MPoint::create();
    return obj;
}

define_reflectable_class_function(MPoint, x, "getter")
define_reflectable_class_function(MPoint, y, "getter")

float MPoint::x() { return mX; }
float MPoint::y() { return mY; }

define_reflectable_class_function(MPoint, equal, "args:that")
bool MPoint::equal(const MPoint::ptr &that) {
    if (that) {
        return mX == that->mX && mY == that->mY;
    } else {
        return !mX && !mY;
    }
}

define_reflectable_class_function(MPoint, add, "args:that")
MPoint::ptr MPoint::add(const MPoint::ptr &that) {
    if (that) {
        return from(mX + that->mX, mY + that->mY);
    }
    return me();
}

define_reflectable_class_function(MPoint, sub, "args:that")
MPoint::ptr MPoint::sub(const MPoint::ptr &that) {
    if (that) {
        return from(mX - that->mX, mY - that->mY);
    }
    return me();
}

//size:

define_reflectable_class_function(MSize, from, "args:width,height")
MSize::ptr MSize::from(float width, float height) {
    if (width || height) {
        auto obj = MSize::create();
        obj->mWidth  = width ;
        obj->mHeight = height;

        return obj;

    } else {
        return zero();
    }
}

define_reflectable_class_function(MSize, zero, "getter")
MSize::ptr MSize::zero() {
    static auto obj = MSize::create();
    return obj;
}

define_reflectable_class_function(MSize, width , "getter")
define_reflectable_class_function(MSize, height, "getter")

float MSize::width () { return mWidth ; }
float MSize::height() { return mHeight; }

define_reflectable_class_function(MSize, none, "getter")
bool MSize::none() {
    return mWidth <= 0 && mHeight <= 0;
}

define_reflectable_class_function(MSize, equal, "args:that")
bool MSize::equal(const MSize::ptr &that) {
    if (that) {
        return mWidth == that->mWidth && mHeight == that->mHeight;
    } else {
        return !mWidth && !mHeight;
    }
}

//rect:

define_reflectable_class_function(MRect, from, "args:x,y,width,height")
MRect::ptr MRect::from(float x, float y, float width, float height) {
    if (x || y || width || height) {
        auto obj = MRect::create();
        obj->mOrigin = MPoint::from(x, y);
        obj->mSize   = MSize ::from(width, height);

        return obj;

    } else {
        return zero();
    }
}

define_reflectable_class_function(MRect, zero, "getter")
MRect::ptr MRect::zero() {
    static auto obj = MRect::create();
    return obj;
}

define_reflectable_class_function(MRect, x     , "getter")
define_reflectable_class_function(MRect, y     , "getter")
define_reflectable_class_function(MRect, width , "getter")
define_reflectable_class_function(MRect, height, "getter")

float MRect::x     () { return mOrigin ? mOrigin->x   () : 0.f; }
float MRect::y     () { return mOrigin ? mOrigin->y   () : 0.f; }
float MRect::width () { return mSize   ? mSize->width () : 0.f; }
float MRect::height() { return mSize   ? mSize->height() : 0.f; }

define_reflectable_class_function(MRect, origin, "getter")
define_reflectable_class_function(MRect, size  , "getter")

MPoint::ptr MRect::origin() { return mOrigin ? mOrigin : MPoint::zero(); }
MSize::ptr  MRect::size  () { return mSize   ? mSize   : MSize ::zero(); }

define_reflectable_class_function(MRect, none, "getter")
bool MRect::none() {
    return !mSize || mSize->none();
}

define_reflectable_class_function(MRect, equal, "args:that")
bool MRect::equal(const MRect::ptr &that) {
    if (that) {
        if (x     () != that->x     ()) { return false; }
        if (y     () != that->y     ()) { return false; }
        if (width () != that->width ()) { return false; }
        if (height() != that->height()) { return false; }

        return true;

    } else {
        if (x     () != 0) { return false; }
        if (y     () != 0) { return false; }
        if (width () != 0) { return false; }
        if (height() != 0) { return false; }

        return true;
    }
}

define_reflectable_class_function(MRect, intersects, "args:rect")
MRect::ptr MRect::intersects(const MRect::ptr &rect) {
    if (none() || !rect || rect->none()) {
        return MRect::zero();
    }
    
    float left   = std::max(x(), rect->x());
    float top    = std::max(y(), rect->y());
    float right  = std::min(x() + width (), rect->x() + rect->width ());
    float bottom = std::min(y() + height(), rect->y() + rect->height());

    if (left < right && top < bottom) {
        return MRect::from(left, top, right - left, bottom - top);
    } else {
        return MRect::zero();
    }
}

define_reflectable_class_function(MRect, contains, "args:point")
bool MRect::contains(const MPoint::ptr &point) {
    if (none() || !point) {
        return false;
    }
    
    return (
        x() <= point->x() && point->x() < x() + width () &&
        y() <= point->y() && point->y() < y() + height()
    );
}
