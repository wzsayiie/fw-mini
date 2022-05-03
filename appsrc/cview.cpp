#include "cview.h"

CView::CView() {
    mSubviews = MVector<CView::ptr>::create();
}

CView::CView(float x, float y, float width, float height) {
    mSubviews = MVector<CView::ptr>::create();
    mFrame = MRect::from(x, y, width, height);
}

define_reflectable_class_function(CView, setFrame, "args:frame")
void CView::setFrame(const MRect::ptr &frame) {
    float oldWidth  = mFrame ? mFrame->width () : 0;
    float oldHeight = mFrame ? mFrame->height() : 0;
    float newWidth  = frame  ? frame ->width () : 0;
    float newHeight = frame  ? frame ->height() : 0;
    
    mFrame = frame;
    
    if (newWidth  != oldWidth  ||
        newHeight != oldHeight )
    {
        for (auto &it : mSubviews->vector) {
            it->onLayoutSubviews(newWidth, newHeight);
        }
    }
}

define_reflectable_class_function(CView, frame)
MRect::ptr CView::frame() {
    return mFrame ? mFrame : MRect::zero();
}

define_reflectable_class_function(CView, bounds)
MRect::ptr CView::bounds() {
    MRect::ptr rect = frame();
    return MRect::from(0, 0, rect->width(), rect->height());
}

define_reflectable_class_function(CView, setBackgroundColor, "args:color")
void CView::setBackgroundColor(const MColor::ptr &color) {
    mBackgroundColor = color;
}

define_reflectable_class_function(CView, backgroundColor)
MColor::ptr CView::backgroundColor() {
    return mBackgroundColor ? mBackgroundColor : MColor::clearColor();
}

define_reflectable_class_function(CView, setAcceptMouseWheel, "args:accept")
void CView::setAcceptMouseWheel(bool accept) {
    mAcceptMouseMove = accept;
}

define_reflectable_class_function(CView, acceptMouseWheel)
bool CView::acceptMouseWheel() {
    return mAcceptMouseMove;
}

define_reflectable_class_function(CView, setTouchable, "args:touchable")
void CView::setTouchable(bool touchable) {
    mTouchable = touchable;
}

define_reflectable_class_function(CView, touchable)
bool CView::touchable() {
    return mTouchable;
}

define_reflectable_class_function(CView, setVisible, "args:visible")
void CView::setVisible(bool visible) {
    mVisiable = visible;
}

define_reflectable_class_function(CView, visible)
bool CView::visible() {
    return mVisiable;
}

define_reflectable_class_function(CView, addSubview, "args:subview")
void CView::addSubview(const CView::ptr &subview) {
    if (!subview) {
        return;
    }
    if (subview.get() == this) {
        return;
    }
    if (subview->mSuperview == this) {
        return;
    }
    
    //remove from old superview.
    if (subview->mSuperview) {
        auto &brothers = subview->mSuperview->mSubviews->vector;
        brothers.erase(std::remove(brothers.begin(), brothers.end(), subview));
    }
    //add to new superview.
    mSubviews->vector.push_back(subview);
    subview->mSuperview = this;
    
    //layout.
    MSize::ptr space = frame()->size();
    onLayoutSubviews(space->width(), space->height());
}

define_reflectable_class_function(CView, removeFromSuperview)
void CView::removeFromSuperview() {
    if (!mSuperview) {
        return;
    }
    
    auto &brothers = mSuperview->mSubviews->vector;
    brothers.erase(std::remove(brothers.begin(), brothers.end(), shared()));
    
    mSuperview = nullptr;
}

define_reflectable_class_function(CView, subviews)
MVector<CView::ptr>::ptr CView::subviews() {
    return mSubviews;
}

define_reflectable_class_function(CView, superview)
CView *CView::superview() {
    return mSuperview;
}

define_reflectable_class_function(CView, findResponder, "args:x,y,fit")
CResponder::ptr CView::findResponder(float x, float y, const CResponderDetector::ptr &fit) {
    implement_injectable_function((CResponder::ptr), fit, x, y)
    
    //find in subviews.
    for (auto &it : mSubviews->vector) {
        MRect::ptr frame = it->frame();
        float nx = x - frame->x();
        float ny = y - frame->y();
        
        CResponder::ptr responder = it->findResponder(nx, ny, fit);
        if (responder) {
            return responder;
        }
    }
    
    //is self suitable.
    auto self = shared();
    if (fit->call(self, x, y)) {
        return self;
    }
    
    return nullptr;
}

define_reflectable_class_function(CView, responseOffset)
MPoint::ptr CView::responseOffset() {
    implement_injectable_function((MPoint::ptr))
    
    float x = frame()->x();
    float y = frame()->y();
    
    for (auto it = mSuperview; it; it = it->mSuperview) {
        x += it->frame()->x();
        y += it->frame()->y();
    }
    
    return MPoint::from(x, y);
}

define_reflectable_class_function(CView, canRespondTouch, "args:x,y")
bool CView::canRespondTouch(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    if (mTouchable) {
        auto point = MPoint::from(x, y);
        return bounds()->contains(point);
    }
    return false;
}

define_reflectable_class_function(CView, canRespondMouseMove, "args:x,y")
bool CView::canRespondMouseMove(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    if (mAcceptMouseMove) {
        auto point = MPoint::from(x, y);
        return bounds()->contains(point);
    }
    return false;
}

define_reflectable_class_function(CView, draw)
void CView::draw() {
    if (!mFrame || mFrame->width() <= 0 || mFrame->height() <= 0) {
        return;
    }
    if (!mVisiable) {
        return;
    }
    
    float x = mFrame->x();
    float y = mFrame->y();
    float width  = mFrame->width ();
    float height = mFrame->height();
    
    MContextPushClip(x, y, width, height);
    MContextPushOffset(x, y);
    {
        onDrawBackground(width, height);
        onDraw(width, height);
        for (auto &it : mSubviews->vector) {
            it->draw();
        }
        onDrawForeground(width, height);
    }
    MContextPopOffset();
    MContextPopClip();
}

define_reflectable_class_function(CView, onLayoutSubviews, "args:width,height")
void CView::onLayoutSubviews(float width, float height) {
    implement_injectable_function((void), width, height)
}

define_reflectable_class_function(CView, onDrawBackground, "args:width,height")
void CView::onDrawBackground(float width, float height) {
    implement_injectable_function((void), width, height)
    
    if (mBackgroundColor && !mBackgroundColor->none()) {
        MContextSelectRGBA(mBackgroundColor->rgba());
        MContextDrawRect(0, 0, width, height);
    }
}

define_reflectable_class_function(CView, onDraw, "args:width,height")
void CView::onDraw(float width, float height) {
    implement_injectable_function((void), width, height)
}

define_reflectable_class_function(CView, onDrawForeground, "args:width,height")
void CView::onDrawForeground(float width, float height) {
    implement_injectable_function((void), width, height)
}
