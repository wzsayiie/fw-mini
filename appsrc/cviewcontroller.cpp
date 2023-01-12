#include "cviewcontroller.h"
#include "cnavigationcontroller.h"
#include "ctabpagecontroller.h"

CViewController::CViewController() {
    mChildControllers = MVector<CViewController::ptr>::create();
}

define_reflectable_class_function(CViewController, makeViewLoad)
void CViewController::makeViewLoad() {
    view();
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
    for (const auto &it : *mChildControllers) {
        it->makeViewAppear();
    }
}

define_reflectable_class_function(CViewController, makeViewDisappear)
void CViewController::makeViewDisappear() {
    if (!mViewAppeared) {
        return;
    }
    
    for (const auto &it : *mChildControllers) {
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

    CViewController::ptr oldParent = childController->mParentController.lock();
    if (oldParent.get() == this) {
        return;
    }
    
    //remove from old parent.
    if (oldParent) {
        auto &brothers = oldParent->mChildControllers;
        brothers->erase(
            std::remove(brothers->begin(), brothers->end(), childController),
            brothers->end()
        );
    }
    //add to new parent:
    view()->addSubview(childController->view());
    
    mChildControllers->push_back(childController);
    childController->mParentController = me();
    
    //adapter appear status.
    if (mViewAppeared) {
        childController->makeViewAppear();
    } else {
        childController->makeViewDisappear();
    }
}

define_reflectable_class_function(CViewController, removeFromParentController)
void CViewController::removeFromParentController() {
    CViewController::ptr nowParent = mParentController.lock();
    if (!nowParent) {
        return;
    }
    
    //remove from the parent:
    auto &brothers = nowParent->mChildControllers;
    brothers->erase(
        std::remove(brothers->begin(), brothers->end(), me()),
        brothers->end()
    );
    mParentController.reset();
    
    view()->removeFromSuperview();
    
    //adpater appear status.
    makeViewDisappear();
}

define_reflectable_class_function(CViewController, childControllers)
MVector<CViewController::ptr>::ptr CViewController::childControllers() {
    return mChildControllers;
}

define_reflectable_class_function(CViewController, parentController)
CViewController::ptr CViewController::parentController() {
    return mParentController.lock();
}

define_reflectable_class_function(CViewController, setNavigationController, "setter;args:controller")
void CViewController::setNavigationController(const CNavigationController::ptr &controller) {
    mNavigationController = controller;
}

define_reflectable_class_function(CViewController, setTabPageController, "setter;args:controller")
void CViewController::setTabPageController(const CTabPageController::ptr &controller) {
    mTabPageController = controller;
}

define_reflectable_class_function(CViewController, navigationController, "getter")
CNavigationController::ptr CViewController::navigationController() {
    return mNavigationController.lock();
}

define_reflectable_class_function(CViewController, tabPageController, "getter")
CTabPageController::ptr CViewController::tabPageController() {
    return mTabPageController.lock();
}

define_reflectable_class_function(CViewController, viewLoaded, "getter")
bool CViewController::viewLoaded() {
    return mView != nullptr;
}

define_reflectable_class_function(CViewController, viewAppeared, "getter")
bool CViewController::viewAppeared() {
    return mViewAppeared;
}

define_reflectable_class_function(CViewController, view, "getter")
CView::ptr CViewController::view() {
    if (!mView) {
        mView = loadView();
        mView->setViewController(me());

        onViewLoad();
    }
    return mView;
}

define_reflectable_class_function(CViewController, findResponder, "virtual;args:evt,pt")
CResponder::ptr CViewController::findResponder(MEvent evt, const MPoint::ptr &pt) {
    implement_injectable_function(CResponder::ptr, evt, pt)

    return view()->findResponder(evt, pt);
}

define_reflectable_class_function(CViewController, existResponder, "virtual;args:responder")
bool CViewController::existResponder(const CResponder::ptr &responder) {
    implement_injectable_function(bool, responder)

    return view()->existResponder(responder);
}

define_reflectable_class_function(CView, responseOffset, "virtual")
MPoint::ptr CViewController::responseOffset()  {
    implement_injectable_function(MPoint::ptr)
    
    return view()->responseOffset();
}

define_reflectable_class_function(CViewController, canBecomeFocusResponder)
bool CViewController::canBecomeFocusResponder() {
    implement_injectable_function(bool)

    return true;
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

define_reflectable_class_function(CViewController, canRespondMouseWheel, "virtual;args:pt")
bool CViewController::canRespondMouseWheel(const MPoint::ptr &pt) {
    implement_injectable_function(bool, pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, canRespondKbKey, "virtual")
bool CViewController::canRespondKbKey() {
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

void CViewController::on_dispose() {
    super::dispose();

    for (const auto &it : *mChildControllers) {
        it->dispose();
    }
    if (mView) {
        mView->dispose();
    }
}
