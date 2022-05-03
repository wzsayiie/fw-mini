#pragma once

#include "cresponder.h"

class CView;
declare_reflectable_special(MVector<std::shared_ptr<CView>>)

declare_reflectable_class(CView)
class CView : public CExtends<CView, CResponder> {
public:
    CView();
    CView(float x, float y, float width, float height);
    
public:
    void setFrame(const MRect::ptr &frame);
    MRect::ptr frame ();
    MRect::ptr bounds();
    
    void setBackgroundColor(const MColor::ptr &color);
    MColor::ptr backgroundColor();
    
    void setAcceptMouseWheel(bool accept);
    bool acceptMouseWheel();

    void setTouchable(bool touchable);
    bool touchable();
    
    void setVisible(bool visible);
    bool visible();
    
    void addSubview(const CView::ptr &subview);
    void removeFromSuperview();
    MVector<CView::ptr>::ptr subviews();
    CView *superview();
    
    CResponder::ptr findResponder(float x, float y, const CResponderDetector::ptr &fit) override;
    MPoint::ptr responseOffset() override;
    
    bool canRespondTouch    (float x, float y) override;
    bool canRespondMouseMove(float x, float y) override;
    
    void draw();
    
protected: public:
    virtual void onLayoutSubviews(float width, float height);
        
    virtual void onDrawBackground(float width, float height);
    virtual void onDraw          (float width, float height);
    virtual void onDrawForeground(float width, float height);
    
private:
    MRect::ptr mFrame;
    MColor::ptr mBackgroundColor;
    bool mAcceptMouseMove = false;
    bool mTouchable = false;
    bool mVisiable = true;
    
    MVector<CView::ptr>::ptr mSubviews;
    CView *mSuperview = nullptr;
};
