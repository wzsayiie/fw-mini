#pragma once

#include "cviewcontroller.h"

m_class(CWindow, MWindow) {
public:
    void setRootViewController(const CViewController::ptr &viewController);
    CViewController::ptr rootViewController();
    
    void makeMainWindow();
    
protected:
    void onLoad() override;
    void onShow() override;
    void onHide() override;

    void onResize(float width, float height) override;
    void onDraw  (float width, float height) override;

    void onTouchBegin(float x, float y) override;
    void onTouchMove (float x, float y) override;
    void onTouchEnd  (float x, float y) override;

    void onMouseMove (float x, float y)              override;
    void onMouseWheel(float x, float y, float delta) override;
    void onKbKey     (MKbKeyCode code)               override;
    void onWriting   (const std::string &text)       override;
    
private:
    CViewController::ptr mRootViewController  ;
    CResponder::ptr      mTouchingResponder   ;
    CResponder::ptr      mMouseMovingResponder;
};
