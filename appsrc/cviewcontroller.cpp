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
    }
    return mView;
}

define_reflectable_class_function(CViewController, findResponder, "args:x,y,fit")
CResponder::ptr CViewController::findResponder(float x, float y, const CResponderDetector::ptr &fit) {
    implement_injectable_function((CResponder::ptr), fit)
    
    //firstly find in the child controllers.
    for (auto &it : mChildControllers->vector) {
        MRect::ptr frame = it->view()->frame();
        float nx = x - frame->x();
        float ny = y - frame->y();
        
        CResponder::ptr responder = it->findResponder(nx, ny, fit);
        if (responder) {
            return responder;
        }
    }

    //secondly find in the views.
    CResponder::ptr responder = view()->findResponder(x, y, fit);
    if (responder) {
        return responder;
    }
    
    //lastly detect self.
    auto self = shared();
    if (fit->call(self, x, y)) {
        return self;
    }
    
    return nullptr;
}

define_reflectable_class_function(CView, responseOffset)
MPoint::ptr CViewController::responseOffset()  {
    implement_injectable_function((MPoint::ptr))
    
    return view()->responseOffset();
}

define_reflectable_class_function(CViewController, canRespondTouch, "args:x,y")
bool CViewController::canRespondTouch(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    auto point = MPoint::from(x, y);
    return view()->bounds()->contains(point);
}

define_reflectable_class_function(CViewController, canRespondMouseMove, "args:x,y")
bool CViewController::canRespondMouseMove(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    auto point = MPoint::from(x, y);
    return view()->bounds()->contains(point);
}

define_reflectable_class_function(CViewController, canRespondWriting, "args:x,y")
bool CViewController::canRespondWriting(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    return true;
}

define_reflectable_class_function(CViewController, canRespondKey, "args:x,y")
bool CViewController::canRespondKey(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    return true;
}

define_reflectable_class_function(CViewController, canRespondWheel, "args:x,y")
bool CViewController::canRespondWheel(float x, float y) {
    implement_injectable_function((bool), x, y)
    
    auto point = MPoint::from(x, y);
    return view()->bounds()->contains(point);
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
