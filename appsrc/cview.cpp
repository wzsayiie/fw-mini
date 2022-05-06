#include "cview.h"

CView::CView() {
    mSubviews = MVector<CView::ptr>::create();
}

CView::CView(float x, float y, float width, float height) {
    mSubviews = MVector<CView::ptr>::create();
    mFrame = MRect::from(x, y, width, height);
}

define_reflectable_class_function(CView, setViewController, "args:controller")
void CView::setViewController(CObject *controller) {
    mViewController = controller;
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
        onLayoutSubviews(newWidth, newHeight);
    }
}

define_reflectable_class_function(CView, setBackgroundColor, "args:color")
void CView::setBackgroundColor(const MColor::ptr &color) {
    mBackgroundColor = color;
}

define_reflectable_class_function(CView, setVisible        , "args:visible"  )
define_reflectable_class_function(CView, setTouchable      , "args:touchable")
define_reflectable_class_function(CView, setAcceptMouseMove, "args:accept"   )
define_reflectable_class_function(CView, setAcceptWheel    , "args:accept"   )

void CView::setVisible        (bool visible  ) { mVisible         = visible  ; }
void CView::setTouchable      (bool touchable) { mTouchable       = touchable; }
void CView::setAcceptMouseMove(bool accept   ) { mAcceptMouseMove = accept   ; }
void CView::setAcceptWheel    (bool accept   ) { mAcceptWheel     = accept   ; }

define_reflectable_class_function(CView, viewController)
CObject *CView::viewController() {
    return mViewController;
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

define_reflectable_class_function(CView, backgroundColor)
MColor::ptr CView::backgroundColor() {
    return mBackgroundColor ? mBackgroundColor : MColor::clearColor();
}

define_reflectable_class_function(CView, visible        )
define_reflectable_class_function(CView, touchable      )
define_reflectable_class_function(CView, acceptMouseMove)
define_reflectable_class_function(CView, acceptWheel    )

bool CView::visible        () { return mVisible        ; }
bool CView::touchable      () { return mTouchable      ; }
bool CView::acceptMouseMove() { return mAcceptMouseMove; }
bool CView::acceptWheel    () { return mAcceptWheel    ; }

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
CResponder::ptr CView::findResponder(const MPoint::ptr &pt, const CResponderDetector::ptr &fit) {
    implement_injectable_function((CResponder::ptr), fit, pt)
    
    //find in subviews.
    for (auto &it : mSubviews->vector) {
        //NOTE: if it is the root view of a view controller, ignore it.
        //this avoids duplicate lookups.
        if (it->viewController()) {
            continue;
        }
        
        MPoint::ptr off = it->frame()->origin();
        MPoint::ptr npt = pt->sub(off);

        CResponder::ptr responder = it->findResponder(npt, fit);
        if (responder) {
            return responder;
        }
    }
    
    //is self suitable.
    auto self = shared();
    if (fit->call(self, pt)) {
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

define_reflectable_class_function(CView, canRespondTouch, "args:pt")
bool CView::canRespondTouch(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    if (mTouchable) {
        return bounds()->contains(pt);
    }
    return false;
}

define_reflectable_class_function(CView, canRespondMouseMove, "args:pt")
bool CView::canRespondMouseMove(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    if (mAcceptMouseMove) {
        return bounds()->contains(pt);
    }
    return false;
}

define_reflectable_class_function(CView, canRespondWheel, "args:pt")
bool CView::canRespondWheel(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)

    if (mAcceptWheel) {
        return bounds()->contains(pt);
    }
    return false;
}

define_reflectable_class_function(CView, draw)
void CView::draw() {
    if (!mFrame || mFrame->width() <= 0 || mFrame->height() <= 0) {
        return;
    }
    if (!mVisible) {
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
