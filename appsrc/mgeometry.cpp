#include "mgeometry.h"

//point:

MPoint::ptr MPoint::from(float x, float y) {
    auto obj = MPoint::create();
    
    obj->mX = x;
    obj->mY = y;
    
    return obj;
}

MPoint::ptr MPoint::zero() {
    static auto obj = MPoint::create();
    return obj;
}

define_reflectable_class_function(MPoint, x)
define_reflectable_class_function(MPoint, y)

float MPoint::x() { return mX; }
float MPoint::y() { return mY; }

define_reflectable_class_function(MPoint, add, "args:that")
MPoint::ptr MPoint::add(const MPoint::ptr &that) {
    if (that) {
        return from(mX + that->mX, mY + that->mY);
    }
    return shared();
}

define_reflectable_class_function(MPoint, sub, "args:that")
MPoint::ptr MPoint::sub(const MPoint::ptr &that) {
    if (that) {
        return from(mX - that->mX, mY - that->mY);
    }
    return shared();
}

//size:

MSize::ptr MSize::from(float width, float height) {
    auto obj = MSize::create();
    
    obj->mWidth  = width ;
    obj->mHeight = height;
    
    return obj;
}

MSize::ptr MSize::zero() {
    static auto obj = MSize::create();
    return obj;
}

define_reflectable_class_function(MSize, width )
define_reflectable_class_function(MSize, height)

float MSize::width () { return mWidth ; }
float MSize::height() { return mHeight; }

define_reflectable_class_function(MSize, none)
bool MSize::none() {
    return mWidth <= 0 && mHeight <= 0;
}

//rect:

MRect::ptr MRect::from(float x, float y, float width, float height) {
    auto obj = MRect::create();
    
    obj->mOrigin = MPoint::from(x, y);
    obj->mSize   = MSize::from(width, height);
    
    return obj;
}

MRect::ptr MRect::zero() {
    static auto obj = MRect::create();
    return obj;
}

define_reflectable_class_function(MRect, x     )
define_reflectable_class_function(MRect, y     )
define_reflectable_class_function(MRect, width )
define_reflectable_class_function(MRect, height)

float MRect::x     () { return mOrigin ? mOrigin->x   () : 0.f; }
float MRect::y     () { return mOrigin ? mOrigin->y   () : 0.f; }
float MRect::width () { return mSize   ? mSize->width () : 0.f; }
float MRect::height() { return mSize   ? mSize->height() : 0.f; }

define_reflectable_class_function(MRect, origin)
define_reflectable_class_function(MRect, size  )

MPoint::ptr MRect::origin() { return mOrigin ? mOrigin : MPoint::zero(); }
MSize::ptr  MRect::size  () { return mSize   ? mSize   : MSize ::zero(); }

define_reflectable_class_function(MRect, none)
bool MRect::none() {
    return mSize ? mSize->none() : true;
}

define_reflectable_class_function(MRect, intersects)
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

define_reflectable_class_function(MRect, contains)
bool MRect::contains(const MPoint::ptr &point) {
    if (none() || !point) {
        return false;
    }
    
    return (
        x() <= point->x() && point->x() < x() + width () &&
        y() <= point->y() && point->y() < y() + height()
    );
}
