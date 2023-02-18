#include "cwindow.h"
#include "rdefine.h"

define_reflectable_class_function(CWindow, setRootViewController, "setter;args:viewController")
void CWindow::setRootViewController(const CViewController::ptr &viewController) {
    mRootViewController = viewController;
}

define_reflectable_class_function(CWindow, rootViewController, "getter")
CViewController::ptr CWindow::rootViewController() {
    if (!mRootViewController) {
        mRootViewController = CViewController::create();
    }
    return mRootViewController;
}

define_reflectable_class_function(CWindow, makeMainWindow)
void CWindow::makeMainWindow() {
    MWindow::setMainWindow(me());
}

void CWindow::onLoad() {
    //ensure the view size is available.
    auto bounds = MRect::from(0, 0,
        size()->width (),
        size()->height()
    );
    rootViewController()->view()->setFrame(bounds);

    rootViewController()->makeViewLoad();
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
    rootViewController()->view()->layer()->draw();
}

void CWindow::onTouchBegin(float x, float y) {
    mTouchingResponder = rootViewController()->findResponder(
        MEvent::Touch, MPoint::from(x, y)
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
    CResponder::ptr responder = rootViewController()->findResponder(
        MEvent::MouseMove, MPoint::from(x, y)
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

void CWindow::onMouseWheel(float x, float y, float delta) {
    CResponder::ptr responder = rootViewController()->findResponder(
        MEvent::MouseWheel, MPoint::from(x, y)
    );
    
    if (responder) {
        MPoint::ptr offset = responder->responseOffset();
        float nx = x - offset->x();
        float ny = y - offset->y();

        responder->onMouseWheel(nx, ny, delta);
    }
}

void CWindow::onKbKey(MKbKeyCode code) {
    CResponder::ptr responder = CResponder::focusResponder();
    if (!responder) {
        return;
    }

    if (!responder->canRespondKbKey()) {
        return;
    }

    responder->onKbKey(code);
}

void CWindow::onWriting(const std::string &text) {
    CResponder::ptr responder = CResponder::focusResponder();
    if (!responder) {
        return;
    }

    if (!responder->canRespondWriting()) {
        return;
    }
    
    responder->onWriting(text);
}

void CWindow::on_dispose() {
    super::dispose();

    if (mRootViewController  ) { mRootViewController  ->dispose(); }
    if (mTouchingResponder   ) { mTouchingResponder   ->dispose(); }
    if (mMouseMovingResponder) { mMouseMovingResponder->dispose(); }
}
