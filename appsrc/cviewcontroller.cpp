#include "cviewcontroller.h"

CViewController::CViewController() {
    mChildControllers = MVector<CViewController::ptr>::create();
}

define_reflectable_class_function(CViewController, makeViewLoad)
void CViewController::makeViewLoad() {
    if (mViewLoaded) {
        return;
    }
    
    mViewLoaded = true;
    onViewLoad();
}

define_reflectable_class_function(CViewController, makeViewAppear)
void CViewController::makeViewAppear() {
    if (mViewAppeared) {
        return;
    }
    
    //NOTE: make sure the view loaded.
    makeViewLoad();
    
    mViewAppeared = true;
    onViewAppear();
    for (auto &it : *mChildControllers) {
        it->makeViewAppear();
    }
}

define_reflectable_class_function(CViewController, makeViewDisappear)
void CViewController::makeViewDisappear() {
    if (!mViewAppeared) {
        return;
    }
    
    for (auto &it : *mChildControllers) {
        it->makeViewDisappear();
    }
    mViewAppeared = false;
    onViewDisappear();
}

define_reflectable_class_function(CViewController, addChildController, "args:childController")
void CViewController::addChildController(const CViewController::ptr &childController) {
    if (!childController) {
        return;
    }
    if (childController.get() == this) {
        return;
    }
    if (childController->mParentController == this) {
        return;
    }
    
    //remove from old parent.
    if (childController->mParentController) {
        auto &brothers = childController->mParentController->mChildControllers;
        brothers->erase(
            std::remove(brothers->begin(), brothers->end(), childController),
            brothers->end()
        );
    }
    //add to new parent:
    view()->addSubview(childController->view());
    
    mChildControllers->push_back(childController);
    childController->mParentController = this;
    
    //adapter appear status.
    if (mViewAppeared) {
        childController->makeViewAppear();
    } else {
        childController->makeViewDisappear();
    }
}

define_reflectable_class_function(CViewController, removeFromParentController)
void CViewController::removeFromParentController() {
    if (!mParentController) {
        return;
    }
    
    //remove from the parent:
    auto &brothers = mParentController->mChildControllers;
    brothers->erase(
        std::remove(brothers->begin(), brothers->end(), shared()),
        brothers->end()
    );
    mParentController = nullptr;
    
    view()->removeFromSuperview();
    
    //adpater appear status.
    makeViewDisappear();
}

define_reflectable_class_function(CViewController, childControllers)
MVector<CViewController::ptr>::ptr CViewController::childControllers() {
    return mChildControllers;
}

define_reflectable_class_function(CViewController, parentController)
CViewController *CViewController::parentController() {
    return mParentController;
}

define_reflectable_class_function(CViewController, view, "getter")
CView::ptr CViewController::view() {
    if (!mView) {
        mView = loadView();
        mView->setViewController(this);
    }
    return mView;
}

define_reflectable_class_function(CViewController, findResponder, "virtual;args:event,pt")
CResponder::ptr CViewController::findResponder(CResponseEvent event, const MPoint::ptr &pt) {
    implement_injectable_function(CResponder::ptr, event, pt)

    return view()->findResponder(event, pt);
}

define_reflectable_class_function(CView, responseOffset, "virtual")
MPoint::ptr CViewController::responseOffset()  {
    implement_injectable_function(MPoint::ptr)
    
    return view()->responseOffset();
}

define_reflectable_class_function(CViewController, canRespondTouch, "virtual;args:pt")
bool CViewController::canRespondTouch(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, canRespondMouseMove, "virtual;args:pt")
bool CViewController::canRespondMouseMove(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, canRespondWheel, "virtual;args:pt")
bool CViewController::canRespondWheel(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, canRespondKey, "virtual")
bool CViewController::canRespondKey() {
    implement_injectable_function(bool)
    
    return true;
}

define_reflectable_class_function(CViewController, canRespondWriting, "virtual")
bool CViewController::canRespondWriting() {
    implement_injectable_function(bool)
    
    return true;
}

define_reflectable_class_function(CViewController, loadView, "virtual")
CView::ptr CViewController::loadView() {
    implement_injectable_function(CView::ptr)
    return CView::create();
}

define_reflectable_class_function(CViewController, onViewLoad     , "virtual")
define_reflectable_class_function(CViewController, onViewAppear   , "virtual")
define_reflectable_class_function(CViewController, onViewDisappear, "virtual")

void CViewController::onViewLoad     () { implement_injectable_function(void) }
void CViewController::onViewAppear   () { implement_injectable_function(void) }
void CViewController::onViewDisappear() { implement_injectable_function(void) }
