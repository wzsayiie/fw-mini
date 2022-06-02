#include "cwindow.h"

define_reflectable_class_function(CWindow, setRootViewController, "type:set;args:viewController")
void CWindow::setRootViewController(const CViewController::ptr &viewController) {
    mRootViewController = viewController;
}

define_reflectable_class_function(CWindow, rootViewController, "type:get")
CViewController::ptr CWindow::rootViewController() {
    if (!mRootViewController) {
        mRootViewController = CViewController::create();
    }
    return mRootViewController;
}

define_reflectable_class_function(CWindow, makeMainWindow)
void CWindow::makeMainWindow() {
    MWindow::setMainWindow(shared());
}

void CWindow::onLoad() {
    //ensure the view size is available.
    auto bounds = MRect::from(0, 0,
        size()->width (),
        size()->height()
    );
    rootViewController()->view()->setFrame(bounds);

    rootViewController()->loadView();
}

void CWindow::onShow() {
    rootViewController()->makeViewAppear();
}

void CWindow::onHide() {
    rootViewController()->makeViewDisappear();
}

void CWindow::onResize(float width, float height) {
    auto frame = MRect::from(0, 0, width, height);
    rootViewController()->view()->setFrame(frame);
}

void CWindow::onDraw(float width, float height) {
    rootViewController()->view()->draw();
}

void CWindow::onTouchBegin(float x, float y) {
    mTouchingResponder = rootViewController()->findResponder(MPoint::from(x, y),
        CResponderDetector::create([](const CResponder::ptr &obj, const MPoint::ptr &npt) {
            return obj->canRespondTouch(npt);
        })
    );
    
    if (mTouchingResponder) {
        MPoint::ptr offset = mTouchingResponder->responseOffset();
        float nx = x - offset->x();
        float ny = y - offset->y();

        mTouchingResponder->onTouchBegin(nx, ny);

        //NOTE: set touching responder as the focus.
        mTouchingResponder->becomeFocusResponder();
    }
}

void CWindow::onTouchMove(float x, float y) {
    if (mTouchingResponder) {
        MPoint::ptr offset = mTouchingResponder->responseOffset();
        float nx = x - offset->x();
        float ny = y - offset->y();

        mTouchingResponder->onTouchMove(nx, ny);
    }
}

void CWindow::onTouchEnd(float x, float y) {
    if (mTouchingResponder) {
        MPoint::ptr offset = mTouchingResponder->responseOffset();
        float nx = x - offset->x();
        float ny = y - offset->y();

        mTouchingResponder->onTouchEnd(nx, ny);
        mTouchingResponder = nullptr;
    }
}

void CWindow::onMouseMove(float x, float y) {
    CResponder::ptr responder = rootViewController()->findResponder(MPoint::from(x, y),
        CResponderDetector::create([](const CResponder::ptr &obj, const MPoint::ptr &npt) {
            return obj->canRespondMouseMove(npt);
        })
    );
    
    //exit last responder.
    if (mMouseMovingResponder && mMouseMovingResponder != responder) {
        MPoint::ptr offset = mMouseMovingResponder->responseOffset();
        float nx = x - offset->x();
        float ny = y - offset->y();

        mMouseMovingResponder->onMouseExit(nx, ny);
    }
    
    //process new responder.
    if (responder) {
        MPoint::ptr offset = responder->responseOffset();
        float nx = x - offset->x();
        float ny = y - offset->y();

        if (responder != mMouseMovingResponder) {
            responder->onMouseEnter(nx, ny);
        } else {
            responder->onMouseMove(nx, ny);
        }
    }
    mMouseMovingResponder = responder;
}

void CWindow::onMouseWheel(float delta) {
    CResponder::ptr responder = rootViewController()->findResponder(mousePosition(),
        CResponderDetector::create([](const CResponder::ptr &obj, const MPoint::ptr &npt) {
            return obj->canRespondWheel(npt);
        })
    );
    
    if (responder) {
        responder->onWheel(delta);
    }
}

void CWindow::onKey(MKey key) {
    CResponder::ptr responder = CResponder::focusResponder();
    if (!responder) {
        return;
    }

    //NOTE: finding with a null "pt" means that mouse position is ignored.
    if (!responder->canRespondKey(nullptr)) {
        return;
    }

    responder->onKey(key);
}

void CWindow::onWrite(const std::string &text, bool enter)  {
    CResponder::ptr responder = CResponder::focusResponder();
    if (!responder) {
        return;
    }

    if (!responder->canRespondWriting(nullptr)) {
        return;
    }
    
    responder->onWrite(text, enter);
}
