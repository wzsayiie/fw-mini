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
    for (auto &it : mChildControllers->vector) {
        it->makeViewAppear();
    }
}

define_reflectable_class_function(CViewController, makeViewDisappear)
void CViewController::makeViewDisappear() {
    if (!mViewAppeared) {
        return;
    }
    
    for (auto &it : mChildControllers->vector) {
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
        auto &brothers = childController->mParentController->mChildControllers->vector;
        brothers.erase(std::remove(brothers.begin(), brothers.end(), childController));
    }
    //add to new parent:
    view()->addSubview(childController->view());
    
    mChildControllers->vector.push_back(childController);
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
    auto &brothers = mParentController->mChildControllers->vector;
    brothers.erase(std::remove(brothers.begin(), brothers.end(), shared()));
    mParentController = nullptr;
    
    view()->removeFromSuperview();
    
    //adpater apear status.
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

define_reflectable_class_function(CViewController, view)
CView::ptr CViewController::view() {
    if (!mView) {
        mView = loadView();
        mView->setViewController(this);
    }
    return mView;
}

define_reflectable_class_function(CViewController, findResponder, "args:x,y,fit")
CResponder::ptr CViewController::findResponder(const MPoint::ptr &pt, const CResponderDetector::ptr &fit) {
    implement_injectable_function((CResponder::ptr), fit)
    
    //firstly find in the child controllers.
    for (auto &it : mChildControllers->vector) {
        MPoint::ptr off = it->view()->frame()->origin();
        MPoint::ptr npt = pt->sub(off);
        
        CResponder::ptr responder = it->findResponder(npt, fit);
        if (responder) {
            return responder;
        }
    }

    //secondly find in the views.
    CResponder::ptr responder = view()->findResponder(pt, fit);
    if (responder) {
        return responder;
    }
    
    //lastly detect self.
    auto self = shared();
    if (fit->call(self, pt)) {
        return self;
    }
    
    return nullptr;
}

define_reflectable_class_function(CView, responseOffset)
MPoint::ptr CViewController::responseOffset()  {
    implement_injectable_function((MPoint::ptr))
    
    return view()->responseOffset();
}

define_reflectable_class_function(CViewController, canRespondTouch, "args:pt")
bool CViewController::canRespondTouch(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, canRespondMouseMove, "args:pt")
bool CViewController::canRespondMouseMove(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, canRespondWriting, "args:pt")
bool CViewController::canRespondWriting(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    return true;
}

define_reflectable_class_function(CViewController, canRespondKey, "args:pt")
bool CViewController::canRespondKey(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    return true;
}

define_reflectable_class_function(CViewController, canRespondWheel, "args:pt")
bool CViewController::canRespondWheel(const MPoint::ptr &pt) {
    implement_injectable_function((bool), pt)
    
    return view()->bounds()->contains(pt);
}

define_reflectable_class_function(CViewController, loadView)
CView::ptr CViewController::loadView() {
    implement_injectable_function((CView::ptr))
    return CView::create();
}

define_reflectable_class_function(CViewController, onViewLoad     )
define_reflectable_class_function(CViewController, onViewAppear   )
define_reflectable_class_function(CViewController, onViewDisappear)

void CViewController::onViewLoad     () { implement_injectable_function((void)) }
void CViewController::onViewAppear   () { implement_injectable_function((void)) }
void CViewController::onViewDisappear() { implement_injectable_function((void)) }
