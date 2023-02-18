#include "cview.h"
#include "cviewcontroller.h"
#include "rdefine.h"

CView::CView(): CView(0, 0, 0, 0) {
}

CView::CView(float x, float y, float width, float height) {
    mLayer = CLayer::create();
    mLayer->setFrame(MRect::from(x, y, width, height));
    
    mLayer->setDrawer     (MF(this, &CView::onDraw     ));
    mLayer->setCoverDrawer(MF(this, &CView::onDrawCover));
    
    mSubviews = MVector<CView::ptr>::create();
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

define_reflectable_class_function(CView, setLayouter, "setter;args:func")
void CView::setLayouter(const MFunction<void ()>::ptr &layouter) {
    mLayouter = layouter;
}

define_reflectable_class_function(CView, setVisible, "setter;args:visible")
void CView::setVisible(bool visible) {
    mLayer->setVisible(visible);
}

define_reflectable_class_function(CView, setFrame, "setter;args:frame")
void CView::setFrame(const MRect::ptr &frame) {
    MSize::ptr oldSize = mLayer->frame()->size();
    mLayer->setFrame(frame);
    
    if (CLayer::inAnimatingAction()) {
        //IMPORTANT: always layout subviews when collecting animation nodes.
        layoutSubviews();
        
    } else {
        MSize::ptr newSize = mLayer->frame()->size();
        if (!newSize->equal(oldSize)) {
            layoutSubviews();
        }
    }
}

define_reflectable_class_function(CView, setBackgroundColor, "setter;args:color")
void CView::setBackgroundColor(const MColor::ptr &color) {
    mLayer->setColor(color);
}

define_reflectable_class_function(CView, setAlpha, "setter;args:alpha")
void CView::setAlpha(float alpha) {
    mLayer->setAlpha(alpha);
}

define_reflectable_class_function(CView, viewController, "getter")
CViewController::ptr CView::viewController() {
    return mViewController.lock();
}

define_reflectable_class_function(CView, interactive, "getter")
bool CView::interactive() {
    return mInteractive;
}

define_reflectable_class_function(CView, layouter, "getter")
MFunction<void ()>::ptr CView::layouter() {
    return mLayouter;
}

define_reflectable_class_function(CView, layer, "getter")
CLayer::ptr CView::layer() {
    return mLayer;
}

define_reflectable_class_function(CView, visible, "getter")
bool CView::visible() {
    return mLayer->visible();
}

define_reflectable_class_function(CView, frame, "getter")
MRect::ptr CView::frame() {
    return mLayer->frame();
}

define_reflectable_class_function(CView, bounds, "getter")
MRect::ptr CView::bounds() {
    MSize::ptr size = mLayer->frame()->size();
    return MRect::from(0, 0, size->width(), size->height());
}

define_reflectable_class_function(CView, backgroundColor, "getter")
MColor::ptr CView::backgroundColor() {
    return mLayer->color();
}

define_reflectable_class_function(CView, alpha, "getter")
float CView::alpha() {
    return mLayer->alpha();
}

define_reflectable_class_function(CView, animate, "args:duration,nextAction")
void CView::animate(double duration, const MFunction<void ()>::ptr &nextAction) {
    mLayer->animate(duration, nextAction);
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
    
    //add to new superview:
    mLayer->addSublayer(subview->layer());
    
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
    
    mLayer->removeFromSuperlayer();
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
    if (mLayer->frame()->none()) {
        return nullptr;
    }
    if (!mLayer->visible()) {
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

define_reflectable_class_function(CView, canBecomeFocusResponder, "virtual")
bool CView::canBecomeFocusResponder() {
    implement_injectable_function(bool)

    return mInteractive;
}

define_reflectable_class_function(CView, canRespondTouch, "virtual;args:pt")
bool CView::canRespondTouch(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return mInteractive && mLayer->visible() && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondMouseMove, "virtual;args:pt")
bool CView::canRespondMouseMove(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return mInteractive && mLayer->visible() && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondMouseWheel, "virtual;args:pt")
bool CView::canRespondMouseWheel(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)

    return mInteractive && mLayer->visible() && bounds()->contains(pt);
}

define_reflectable_class_function(CView, canRespondKbKey, "virtual")
bool CView::canRespondKbKey() {
    implement_injectable_function(bool)

    return mInteractive && mLayer->visible();
}

define_reflectable_class_function(CView, canRespondWriting, "virtual")
bool CView::canRespondWriting() {
    implement_injectable_function(bool)

    return mInteractive && mLayer->visible();
}

define_reflectable_class_function(CView, onLayoutSubviews, "virtual;args:width,height")
void CView::onLayoutSubviews(float width, float height) {
    implement_injectable_function(void, width, height)
}

define_reflectable_class_function(CView, onDraw, "virtual;args:width,height")
void CView::onDraw(float width, float height) {
    implement_injectable_function(void, width, height)
}

define_reflectable_class_function(CView, onDrawCover, "virtual;args:width,height")
void CView::onDrawCover(float width, float height) {
    implement_injectable_function(void, width, height)
}

void CView::layoutSubviews() {
    if (mLayouter) {
        mLayouter->call();

    } else {
        MSize::ptr space = frame()->size();
        onLayoutSubviews(space->width(), space->height());
    }
}

void CView::on_dispose() {
    super::dispose();

    mLayouter = nullptr;
    mLayer->dispose();
    for (const auto &it : *mSubviews) {
        it->dispose();
    }
}
