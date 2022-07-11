#include "cview.h"

CView::CView() {
    mSubviews = MVector<CView::ptr>::create();
}

CView::CView(float x, float y, float width, float height) {
    mSubviews = MVector<CView::ptr>::create();
    mFrame = MRect::from(x, y, width, height);
}

define_reflectable_class_function(CView, setViewController, "setter;args:controller")
void CView::setViewController(CObject *controller) {
    mViewController = controller;
}

define_reflectable_class_function(CView, setFrame, "setter;args:frame")
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

define_reflectable_class_function(CView, setBackgroundColor, "setter;args:color")
void CView::setBackgroundColor(const MColor::ptr &color) {
    mBackgroundColor = color;
}

define_reflectable_class_function(CView, setVisible        , "setter;args:visible"  )
define_reflectable_class_function(CView, setTouchable      , "setter;args:touchable")
define_reflectable_class_function(CView, setAcceptMouseMove, "setter;args:accept"   )
define_reflectable_class_function(CView, setAcceptWheel    , "setter;args:accept"   )

void CView::setVisible        (bool visible  ) { mVisible         = visible  ; }
void CView::setTouchable      (bool touchable) { mTouchable       = touchable; }
void CView::setAcceptMouseMove(bool accept   ) { mAcceptMouseMove = accept   ; }
void CView::setAcceptWheel    (bool accept   ) { mAcceptWheel     = accept   ; }

define_reflectable_class_function(CView, viewController, "getter")
CObject *CView::viewController() {
    return mViewController;
}

define_reflectable_class_function(CView, frame, "getter")
MRect::ptr CView::frame() {
    return mFrame ? mFrame : MRect::zero();
}

define_reflectable_class_function(CView, bounds, "getter")
MRect::ptr CView::bounds() {
    MRect::ptr rect = frame();
    return MRect::from(0, 0, rect->width(), rect->height());
}

define_reflectable_class_function(CView, backgroundColor, "getter")
MColor::ptr CView::backgroundColor() {
    return mBackgroundColor ? mBackgroundColor : MColor::clearColor();
}

define_reflectable_class_function(CView, visible        , "getter")
define_reflectable_class_function(CView, touchable      , "getter")
define_reflectable_class_function(CView, acceptMouseMove, "getter")
define_reflectable_class_function(CView, acceptWheel    , "getter")

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
        auto &brothers = subview->mSuperview->mSubviews;
        brothers->erase(
            std::remove(brothers->begin(), brothers->end(), subview),
            brothers->end()
        );
    }
    //add to new superview.
    mSubviews->push_back(subview);
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
    
    auto &brothers = mSuperview->mSubviews;
    brothers->erase(
        std::remove(brothers->begin(), brothers->end(), shared()),
        brothers->end()
    );
    
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

define_reflectable_class_function(CView, findResponder, "virtual;args:event,pt")
CResponder::ptr CView::findResponder(CResponseEvent event, const MPoint::ptr &pt) {
    implement_injectable_function(CResponder::ptr, event, pt)
    
    //untouchable views ignore cursor events.
    if (!mTouchable) {
        if (event == CResponseEvent::Touch     ||
            event == CResponseEvent::MouseMove ||
            event == CResponseEvent::Wheel     )
        {
            return nullptr;
        }
    }
    //invisible view does not respond any events.
    if (!mFrame || mFrame->none()) {
        return nullptr;
    }
    if (!mVisible) {
        return nullptr;
    }
    //view does not respond events outside it.
    MRect::ptr ownBounds = bounds();
    if (pt && !ownBounds->contains(pt)) {
        return nullptr;
    }

    //find in subviews.
    for (auto &it : *mSubviews) {
        //NOTE: if it is the root view of a view controller, ignore it.
        //this avoids duplicate lookups.
        if (it->viewController()) {
            continue;
        }
        //ignore out-of-bounds subviews.
        if (it->frame()->intersects(ownBounds)->none()) {
            continue;
        }
        
        MPoint::ptr off = it->frame()->origin();
        MPoint::ptr npt = pt->sub(off);

        CResponder::ptr responder = it->findResponder(event, npt);
        if (responder) {
            return responder;
        }
    }
    
    //is self suitable.
    if (canRespond(event, pt)) {
        return shared();
    }
    
    return nullptr;
}

define_reflectable_class_function(CView, responseOffset, "virtual")
MPoint::ptr CView::responseOffset() {
    implement_injectable_function(MPoint::ptr)
    
    float x = frame()->x();
    float y = frame()->y();
    
    for (auto it = mSuperview; it; it = it->mSuperview) {
        x += it->frame()->x();
        y += it->frame()->y();
    }
    
    return MPoint::from(x, y);
}

define_reflectable_class_function(CView, canRespondTouch, "virtual;args:pt")
bool CView::canRespondTouch(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return mTouchable;
}

define_reflectable_class_function(CView, canRespondMouseMove, "virtual;args:pt")
bool CView::canRespondMouseMove(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return mAcceptMouseMove;
}

define_reflectable_class_function(CView, canRespondWheel, "virtual;args:pt")
bool CView::canRespondWheel(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)

    return mAcceptWheel;
}

define_reflectable_class_function(CView, draw)
void CView::draw() {
    if (!mFrame || mFrame->none()) {
        return;
    }
    if (!mVisible) {
        return;
    }
    
    float x      = mFrame->x();
    float y      = mFrame->y();
    float width  = mFrame->width ();
    float height = mFrame->height();
    
    MContextPushOffset(x, y);
    MContextPushClip  (0, 0, width, height);
    {
        onDrawBackground(width, height);
        onDraw(width, height);

        MRect::ptr ownBounds = bounds();
        for (auto &it : *mSubviews) {
            //NOTE: ignore out-of-bounds subviews.
            if (it->frame()->intersects(ownBounds)->none()) {
                continue;
            }

            it->draw();
        }
        
        onDrawForeground(width, height);
    }
    MContextPopClip  ();
    MContextPopOffset();
}

define_reflectable_class_function(CView, onLayoutSubviews, "virtual;args:width,height")
void CView::onLayoutSubviews(float width, float height) {
    implement_injectable_function(void, width, height)
}

define_reflectable_class_function(CView, onDrawBackground, "virtual;args:width,height")
void CView::onDrawBackground(float width, float height) {
    implement_injectable_function(void, width, height)
    
    if (mBackgroundColor && !mBackgroundColor->none()) {
        MContextSelectRGBA(mBackgroundColor->rgba());
        MContextDrawRect(0, 0, width, height);
    }
}

define_reflectable_class_function(CView, onDraw, "virtual;args:width,height")
void CView::onDraw(float width, float height) {
    implement_injectable_function(void, width, height)
}

define_reflectable_class_function(CView, onDrawForeground, "virtual;args:width,height")
void CView::onDrawForeground(float width, float height) {
    implement_injectable_function(void, width, height)
}
