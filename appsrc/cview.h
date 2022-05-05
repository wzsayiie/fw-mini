#pragma once

#include "cresponder.h"

class CView;
declare_reflectable_special(MVector<std::shared_ptr<CView>>)

c_class(CView, CResponder) {
public:
    CView();
    CView(float x, float y, float width, float height);
    
public: 
    void setViewController(CObject *controller);
    void setFrame(const MRect::ptr &frame);
    void setBackgroundColor(const MColor::ptr &color);
    void setAcceptMouseMove(bool accept);
    void setAcceptWheel(bool accept);
    void setTouchable(bool touchable);
    void setVisible(bool visible);

    CObject    *viewController();
    MRect::ptr  frame();
    MColor::ptr backgroundColor();
    MRect::ptr  bounds();
    bool        acceptMouseMove();
    bool        acceptWheel();
    bool        touchable();
    bool        visible();
    
    void addSubview(const CView::ptr &subview);
    void removeFromSuperview();
    MVector<CView::ptr>::ptr subviews();
    CView *superview();
    
    CResponder::ptr findResponder(const MPoint::ptr &pt, const CResponderDetector::ptr &fit) override;
    MPoint::ptr responseOffset() override;
    
    bool canRespondTouch    (const MPoint::ptr &pt) override;
    bool canRespondMouseMove(const MPoint::ptr &pt) override;
    bool canRespondWheel    (const MPoint::ptr &pt) override;
    
    void draw();
    
protected: public:
    virtual void onLayoutSubviews(float width, float height);

    virtual void onDrawBackground(float width, float height);
    virtual void onDraw          (float width, float height);
    virtual void onDrawForeground(float width, float height);
    
private:
    CObject    *mViewController  = nullptr;
    MRect::ptr  mFrame           ;
    MColor::ptr mBackgroundColor ;
    bool        mAcceptMouseMove = false;
    bool        mAcceptWheel     = false;
    bool        mTouchable       = false;
    bool        mVisiable        = true;
    
    MVector<CView::ptr>::ptr mSubviews;
    CView *mSuperview = nullptr;
};
