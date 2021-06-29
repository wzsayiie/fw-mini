#include "cviewcontroller.h"
#include "mdebug.h"
#include "mhostui.h"

static CViewController *sMainController = nullptr;
static MLambda *sWindowListener = nullptr;

CViewController::~CViewController() {
    if (sMainController == this) {
        sMainController = nullptr;
    }
}

void CViewController::asMainController() {
    if (sMainController) {
        D("can't set main view controller repeatedly");
        return;
    }
    
    sMainController = this;
    sWindowListener = MLambdaCreate(handleWindowEvent, nullptr);
    MWindowAddListener(sWindowListener);
}

static void RemoveChild(std::vector<CViewControllerRef> *children, CViewController *child) {
    for (auto it = children->begin(); it != children->end(); ++it) {
        if (it->get() == child) {
            children->erase(it);
            break;
        }
    }
}

void CViewController::addChildController(CViewControllerRef childController) {
    if (!childController) {
        return;
    }
    
    //add the child to own children list.
    if (childController->mParentController) {
        RemoveChild(&childController->mParentController->mChildControllers, childController.get());
    }
    mChildControllers.push_back(childController);
    childController->mParentController = this;
    
    childController->handleViewLoad();
    childController->mView->removeFromSuperview();
    handleViewLoad();
    mView->addSubview(childController->mView);
    
    //"show" event.
    if (mViewShown) {
        childController->handleViewShow();
    }
}

void CViewController::removeFromParentController() {
    if (!mParentController) {
        return;
    }
    
    //remove self from parent.
    RemoveChild(&mParentController->mChildControllers, this);
    mParentController = nullptr;
    
    mView->removeFromSuperview();
    
    //"hide" event.
    handleViewHide();
}

const std::vector<CViewControllerRef> &CViewController::childControllers() {
    return mChildControllers;
}

CViewController *CViewController::parentController() {
    return mParentController;
}

CViewRef CViewController::view() {
    if (!mView) {
        mView = loadView();
    }
    return mView;
}

CViewRef CViewController::loadView() {
    return CViewRef(new CView);
}

void CViewController::viewLoad() {
}

void CViewController::viewShow() {
}

void CViewController::viewHide() {
}

void CViewController::viewResize(float width, float height) {
}

void CViewController::handleViewLoad() {
    if (mViewLoaded) {
        return;
    }
    
    view();
    mViewLoaded = true;
    viewLoad();
}

void CViewController::handleViewShow() {
    if (mViewShown) {
        return;
    }
    
    mViewShown = true;
    viewShow();
    for (CViewControllerRef child : mChildControllers) {
        child->handleViewShow();
    }
}

void CViewController::handleViewHide() {
    if (!mViewShown) {
        return;
    }
    
    mViewShown = false;
    viewShow();
    for (CViewControllerRef child : mChildControllers) {
        child->handleViewShow();
    }
}

void CViewController::handleWindowEvent(MObject *, MObject *) {
    MWindowEvent event = MWindowCurrentEvent();
    
    if (event == MWindowEvent_Load && !sMainController) {
        D("must set the main view controller before the 'MWindowEvent_Load' event.");
    }
    
    if (!sMainController) {
        return;
    }
    switch (event) {
        case MWindowEvent_Load      : sMainController->handleWindowLoad      (); break;
        case MWindowEvent_Show      : sMainController->handleViewShow        (); break;
        case MWindowEvent_Hide      : sMainController->handleViewHide        (); break;
        case MWindowEvent_Resize    : sMainController->handleWindowResize    (); break;
        case MWindowEvent_Draw      : sMainController->handleWindowDraw      (); break;
        case MWindowEvent_TouchBegin: sMainController->handleWindowTouchBegin(); break;
        case MWindowEvent_TouchMove : sMainController->handleWindowTouchMove (); break;
        case MWindowEvent_TouchEnd  : sMainController->handleWindowTouchEnd  (); break;
        case MWindowEvent_TextBox   : sMainController->handleWindowTextBox   (); break;
        case MWindowEvent_KeyDown   : sMainController->handleWindowKeyDown   (); break;
        default:;
    }
}

void CViewController::handleWindowLoad() {
    float width  = MWindowWidth ();
    float height = MWindowHeight();
    view()->setSize(width, height);
    
    handleViewLoad();
}

void CViewController::handleWindowResize() {
    float width  = MWindowWidth ();
    float height = MWindowHeight();
    mView->setSize(width, height);
    
    viewResize(width, height);
}

void CViewController::handleWindowDraw() {
}

void CViewController::handleWindowTouchBegin() {
}

void CViewController::handleWindowTouchMove() {
}

void CViewController::handleWindowTouchEnd() {
}

void CViewController::handleWindowTextBox() {
}

void CViewController::handleWindowKeyDown() {
}
