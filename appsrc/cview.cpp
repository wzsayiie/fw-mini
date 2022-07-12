#include "cview.h"
#include "cviewcontroller.h"

CView::CView() {
    mSubviews = MVector<CView::ptr>::create();
}

CView::CView(float x, float y, float width, float height) {
    mSubviews = MVector<CView::ptr>::create();
    mFrame = MRect::from(x, y, width, height);
}

define_reflectable_class_function(CView, setViewController , "setter;args:controller" )
define_reflectable_class_function(CView, setInteractive    , "setter;args:interactive")
define_reflectable_class_function(CView, setVisible        , "setter;args:visible"    )
define_reflectable_class_function(CView, setBackgroundColor, "setter;args:color"      )

void CView::setViewController (CViewController   *controller ) { mViewController  = controller ; }
void CView::setInteractive    (bool               interactive) { mInteractive     = interactive; }
void CView::setVisible        (bool               visible    ) { mVisible         = visible    ; }
void CView::setBackgroundColor(const MColor::ptr &color      ) { mBackgroundColor = color      ; }

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

define_reflectable_class_function(CView, viewController, "getter")
CViewController *CView::viewController() {
    return mViewController;
}

define_reflectable_class_function(CView, interactive, "getter")
bool CView::interactive() {
    return mInteractive;
}

define_reflectable_class_function(CView, visible, "getter")
bool CView::visible() {
    return mVisible;
}

define_reflectable_class_function(CView, backgroundColor, "getter")
MColor::ptr CView::backgroundColor() {
    return mBackgroundColor ? mBackgroundColor : MColor::clearColor();
}

define_reflectable_class_function(CView, bounds, "getter")
MRect::ptr CView::bounds() {
    if (mFrame) {
        return MRect::from(0, 0, mFrame->width(), mFrame->height());
    }
    return MRect::zero();
}

define_reflectable_class_function(CView, frame, "getter")
MRect::ptr CView::frame() {
    return mFrame ? mFrame : MRect::zero();
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
    
    //events outside of view are not accepted.
    if (!bounds()->contains(pt)) {
        return nullptr;
    }

    //ignore invisible views.
    if (!mFrame || mFrame->none()) {
        return nullptr;
    }
    if (!mVisible) {
        return nullptr;
    }

    //test subviews.
    for (auto &it : *mSubviews) {
        MPoint::ptr off = it->frame()->origin();
        MPoint::ptr npt = pt->sub(off);

        CResponder::ptr responder = it->findResponder(event, npt);
        if (responder) {
            return responder;
        }
    }

    //test self.
    if (canRespond(event, pt)) {
        return shared();
    }

    //test controller.
    if (mViewController && mViewController->canRespond(event, pt)) {
        return mViewController->shared();
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
    
    return mInteractive && mVisible && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondMouseMove, "virtual;args:pt")
bool CView::canRespondMouseMove(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return mInteractive && mVisible && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondWheel, "virtual;args:pt")
bool CView::canRespondWheel(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)

    return mInteractive && mVisible && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondKey)
bool CView::canRespondKey() {
    implement_injectable_function(bool)

    return mInteractive && mVisible;
}

define_reflectable_class_function(CView, canRespondWriting)
bool CView::canRespondWriting() {
    implement_injectable_function(bool)

    return mInteractive && mVisible;
}

define_reflectable_class_function(CView, draw)
void CView::draw() {
    //subviews do not go beyond the superview's bounds.
    if (!mFrame || mFrame->none()) {
        return;
    }

    //if a view is not visible, all its subviews are also not visible.
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
            //ignore out-of-bounds subviews.
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
