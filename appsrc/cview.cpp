#include "cview.h"
#include "cviewcontroller.h"
#include "mcontext.h"
#include "rdefine.h"

CView::CView() {
    mSubviews = MVector<CView::ptr>::create();
}

CView::CView(float x, float y, float width, float height) {
    mSubviews = MVector<CView::ptr>::create();
    mFrame = MRect::from(x, y, width, height);
}

define_reflectable_class_function(CView, setViewController, "setter;args:controller")
void CView::setViewController(const CViewController::ptr &controller) {
    mViewController = controller;
}

define_reflectable_class_function(CView, setInteractive, "setter;args:interactive")
void CView::setInteractive(bool interactive) {
    mInteractive = interactive;

    if (!interactive && isFocusResponder()) {
        resignFocusResponder();
    }
}

define_reflectable_class_function(CView, setVisible, "setter;args:visible")
void CView::setVisible(bool visible) {
    mVisible = visible;
}

define_reflectable_class_function(CView, setBackgroundColor, "setter;args:color")
void CView::setBackgroundColor(const MColor::ptr &color) {
    mBackgroundColor = color;
}

define_reflectable_class_function(CView, setLayoutDelegation, "setter;args:delegation")
void CView::setLayoutDelegation(const MFunction<void ()>::ptr &delegation) {
    mLayoutDelegation = delegation;
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
        layoutSubviews();
    }
}

define_reflectable_class_function(CView, viewController, "getter")
CViewController::ptr CView::viewController() {
    return mViewController.lock();
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

define_reflectable_class_function(CView, layoutDelegation, "getter")
MFunction<void ()>::ptr CView::layoutDelegation() {
    return mLayoutDelegation;
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

    CView::ptr oldSuper = subview->mSuperview.lock();
    if (oldSuper.get() == this) {
        return;
    }
    
    //remove from old superview.
    if (oldSuper) {
        auto &brothers = oldSuper->mSubviews;
        brothers->erase(
            std::remove(brothers->begin(), brothers->end(), subview),
            brothers->end()
        );
    }
    //add to new superview.
    mSubviews->push_back(subview);
    subview->mSuperview = me();
    
    //layout.
    layoutSubviews();
}

define_reflectable_class_function(CView, removeFromSuperview)
void CView::removeFromSuperview() {
    CView::ptr nowSuper = mSuperview.lock();
    if (!nowSuper) {
        return;
    }
    
    auto &brothers = nowSuper->mSubviews;
    brothers->erase(
        std::remove(brothers->begin(), brothers->end(), me()),
        brothers->end()
    );
    
    mSuperview.reset();
}

define_reflectable_class_function(CView, subviews, "getter")
MVector<CView::ptr>::ptr CView::subviews() {
    return mSubviews;
}

define_reflectable_class_function(CView, superview, "getter")
CView::ptr CView::superview() {
    return mSuperview.lock();
}

define_reflectable_class_function(CView, findResponder, "virtual;args:evt,pt")
CResponder::ptr CView::findResponder(MEvent evt, const MPoint::ptr &pt) {
    implement_injectable_function(CResponder::ptr, evt, pt)
    
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

    //test subviews (NOTE: find from the back).
    for (auto it = mSubviews->rbegin(); it != mSubviews->rend(); ++it) {
        MPoint::ptr off = (*it)->frame()->origin();
        MPoint::ptr npt = pt->sub(off);

        CResponder::ptr responder = (*it)->findResponder(evt, npt);
        if (responder) {
            return responder;
        }
    }

    //test self.
    if (canRespond(evt, pt)) {
        return me();
    }

    //test controller.
    CViewController::ptr controller = mViewController.lock();
    if (controller && controller->canRespond(evt, pt)) {
        return controller;
    }

    return nullptr;
}

define_reflectable_class_function(CView, existResponder, "virtual;args:responder")
bool CView::existResponder(const CResponder::ptr &responder) {
    implement_injectable_function(bool, responder)
    
    if (!responder) {
        return false;
    }

    //check self.
    if (mViewController.lock().get() == responder.get()) {
        return true;
    }
    if (this == responder.get()) {
        return true;
    }

    //check subviews.
    for (const auto &it : *mSubviews) {
        if (it->existResponder(responder)) {
            return true;
        }
    }

    return false;
}

define_reflectable_class_function(CView, responseOffset, "virtual")
MPoint::ptr CView::responseOffset() {
    implement_injectable_function(MPoint::ptr)
    
    float x = frame()->x();
    float y = frame()->y();
    
    for (auto it = mSuperview.lock(); it; it = it->mSuperview.lock()) {
        x += it->frame()->x();
        y += it->frame()->y();
    }
    
    return MPoint::from(x, y);
}

define_reflectable_class_function(CView, canBecomeFocusResponder)
bool CView::canBecomeFocusResponder() {
    implement_injectable_function(bool)

    return mInteractive;
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

define_reflectable_class_function(CView, canRespondMouseWheel, "virtual;args:pt")
bool CView::canRespondMouseWheel(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)

    return mInteractive && mVisible && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondKbKey)
bool CView::canRespondKbKey() {
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
        for (const auto &it : *mSubviews) {
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

void CView::layoutSubviews() {
    if (mLayoutDelegation) {
        mLayoutDelegation->call();

    } else {
        MSize::ptr space = frame()->size();
        onLayoutSubviews(space->width(), space->height());
    }
}

void CView::on_dispose() {
    super::dispose();

    if (mLayoutDelegation) {
        mLayoutDelegation = nullptr;
    }
    for (const auto &it : *mSubviews) {
        it->dispose();
    }
}
