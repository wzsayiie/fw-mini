#include "cwindow.h"

define_reflectable_class_function(CWindow, setRootViewController, "args:viewController")
void CWindow::setRootViewController(const CViewController::ptr &viewController) {
    mRootViewController = viewController;
}

define_reflectable_class_function(CWindow, rootViewController)
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

void CWindow::onLoad() { rootViewController()->loadView         (); }
void CWindow::onShow() { rootViewController()->makeViewAppear   (); }
void CWindow::onHide() { rootViewController()->makeViewDisappear(); }

void CWindow::onResize(float width, float height) {
    auto frame = MRect::from(0, 0, width, height);
    rootViewController()->view()->setFrame(frame);
}

void CWindow::onDraw(float width, float height) {
    rootViewController()->view()->draw();
}

void CWindow::onTouchBegin(float x, float y) {
    auto fit = CResponderDetector::create([](const CResponder::ptr &obj, float nx, float ny) {
        return obj->canRespondTouch(nx, ny);
    });
    mTouchingResponder = rootViewController()->findResponder(x, y, fit);
    
    if (mTouchingResponder) {
        MPoint::ptr point = mTouchingResponder->responseOffset();
        mTouchingResponder->onTouchBegin(point->x(), point->y());
    }
}

void CWindow::onTouchMove(float x, float y) {
    if (mTouchingResponder) {
        MPoint::ptr point = mTouchingResponder->responseOffset();
        mTouchingResponder->onTouchMove(point->x(), point->y());
    }
}

void CWindow::onTouchEnd(float x, float y) {
    if (mTouchingResponder) {
        MPoint::ptr point = mTouchingResponder->responseOffset();
        mTouchingResponder->onTouchEnd(point->x(), point->y());
        
        mTouchingResponder = nullptr;
    }
}

void CWindow::onMouseMove(float x, float y) {
    auto fit = CResponderDetector::create([](const CResponder::ptr &obj, float nx, float ny) {
        return obj->canRespondMouseMove(nx, ny);
    });
    CResponder::ptr responder = rootViewController()->findResponder(x, y, fit);
    
    //exit last responder.
    if (mMouseMovingResponder && mMouseMovingResponder != responder) {
        MPoint::ptr point = mMouseMovingResponder->responseOffset();
        mMouseMovingResponder->onMouseExit(x - point->x(), y - point->y());
    }
    
    //process new responder.
    if (responder) {
        MPoint::ptr point = responder->responseOffset();
        if (responder != mMouseMovingResponder) {
            responder->onMouseEnter(x - point->x(), y - point->y());
        } else {
            responder->onMouseMove (x - point->x(), y - point->y());
        }
    }
    mMouseMovingResponder = responder;
}

void CWindow::onMouseWheel(float delta) {
    auto fit = CResponderDetector::create([](const CResponder::ptr &obj, float nx, float ny) {
        return obj->canRespondWheel(nx, ny);
    });
    MPoint::ptr mouse = mousePosition();
    CResponder::ptr responder = rootViewController()->findResponder(mouse->x(), mouse->y(), fit);
    
    if (responder) {
        responder->onWheel(delta);
    }
}

void CWindow::onKey(MKey key) {
    auto fit = CResponderDetector::create([](const CResponder::ptr &obj, float, float) {
        return obj->canRespondKey(0, 0);
    });
    CResponder::ptr responder = rootViewController()->findResponder(0, 0, fit);
    
    if (responder) {
        responder->onKey(key);
    }
}

void CWindow::onWrite(const std::string &text, bool enter)  {
    auto fit = CResponderDetector::create([](const CResponder::ptr &obj, float, float) {
        return obj->canRespondWriting(0, 0);
    });
    CResponder::ptr responder = rootViewController()->findResponder(0, 0, fit);
    
    if (responder) {
        responder->onWrite(text, enter);
    }
}
