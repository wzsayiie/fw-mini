#pragma once

#include "cviewcontroller.h"

minikit_class(CWindow, MWindow) {
public:
    void setRootViewController(const CViewController::ptr &viewController);
    CViewController::ptr rootViewController();
    
    void makeMainWindow();
    
private:
    void onLoad() override;
    void onShow() override;
    void onHide() override;

    void onResize(float width, float height) override;
    void onDraw  (float width, float height) override;

    void onTouchBegin(float x, float y) override;
    void onTouchMove (float x, float y) override;
    void onTouchEnd  (float x, float y) override;
    void onMouseMove (float x, float y) override;
    
    void onMouseWheel(float delta) override;
    void onKey(MKey key) override;
    void onWrite(const std::string &text, bool enter) override;
    
private:
    CViewController::ptr mRootViewController  ;
    CResponder::ptr      mTouchingResponder   ;
    CResponder::ptr      mMouseMovingResponder;
};
