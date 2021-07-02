#include "cviewcontroller.h"
#include "mdebug.h"
#include "mhostui.h"

static CViewController *sRootController = nullptr;
static MLambda *sWindowListener = nullptr;

CViewController::~CViewController() {
    if (this == sRootController) {
        sRootController = nullptr;
    }
}

void CViewController::asRootController() {
    if (!sWindowListener) {
        sWindowListener = MLambdaCreate(handleWindowEvent, nullptr);
        MWindowAddListener(sWindowListener);
    }
    sRootController = this;
    view()->asRootView();

    if (MWindowLoaded()) {
        handleLoad();
        handleAppear();
    }
}

static void RemoveChildController(std::vector<CViewControllerRef> *children, CViewController *controller) {
    for (auto it = children->begin(); it != children->end(); ++it) {
        if (it->get() == controller) {
            children->erase(it);
            break;
        }
    }
}

void CViewController::addChildController(CViewControllerRef childController) {
    if (!childController) {
        return;
    }
    if (childController->mParentController == this) {
        return;
    }

    //make sure the views are loaded.
    childController->handleLoad();
    handleLoad();

    //move the child controller.
    CViewController *oldParent = childController->mParentController;
    if (oldParent) {
        RemoveChildController(&oldParent->mChildControllers, childController.get());
        childController->mView->removeFromSuperview();
    }
    mChildControllers.push_back(childController);
    childController->mParentController = this;
    mView->addSubview(childController->mView);

    //send "appear" event if needed.
    if (mViewAppeared) {
        childController->handleAppear();
    }
}

void CViewController::removeFromParentController() {
    if (!mParentController) {
        return;
    }

    //remove from the parent.
    RemoveChildController(&mParentController->mChildControllers, this);
    mParentController = nullptr;
    mView->removeFromSuperview();

    //send "disappear" event.
    handleDisappear();
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
        onViewLoad();
    }
    return mView;
}

CViewRef CViewController::loadView() {
    return CViewRef(new CView(0, 0, 0, 0));
}

void CViewController::handleWindowEvent(MObject *, MObject *) {
    if (!sRootController) {
        return;
    }

    MWindowEvent event = MWindowCurrentEvent();
    if (event == MWindowEvent_Load) {
        float width  = MWindowWidth ();
        float height = MWindowHeight();
        sRootController->view()->setSize(width, height);

        sRootController->handleLoad();
        sRootController->handleAppear();
    }
}

void CViewController::handleLoad() {
    if (mViewLoaded) {
        return;
    }

    view();
    mViewLoaded = true;
    onViewLoad();
}

void CViewController::handleAppear() {
    if (!mViewAppeared) {
        mViewAppeared = true;
        onViewAppear();
    }
}

void CViewController::handleDisappear() {
    if (mViewAppeared) {
        mViewAppeared = false;
        onViewDisappear();
    }
}
