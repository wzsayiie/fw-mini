#pragma once

#include "cresponder.h"

class CViewController;

c_class(CView, CResponder) {
public:
    CView();
    CView(float x, float y, float width, float height);
    
public:
    void setViewController (CViewController   *controller );
    void setInteractive    (bool               interactive);
    void setVisible        (bool               visible    );
    void setBackgroundColor(const MColor::ptr &color      );
    void setFrame          (const MRect::ptr  &frame      );

    CViewController *viewController ();
    bool             interactive    ();
    bool             visible        ();
    MColor::ptr      backgroundColor();
    MRect::ptr       bounds         ();
    MRect::ptr       frame          ();
    
    void addSubview(const CView::ptr &subview);
    void removeFromSuperview();
    MVector<CView::ptr>::ptr subviews();
    CView *superview();
    
    CResponder::ptr findResponder(CResponseEvent event, const MPoint::ptr &pt) override;
    MPoint::ptr responseOffset() override;
    
    bool canRespondTouch    (const MPoint::ptr &pt) override;
    bool canRespondMouseMove(const MPoint::ptr &pt) override;
    bool canRespondWheel    (const MPoint::ptr &pt) override;
    bool canRespondKey      ()                      override;
    bool canRespondWriting  ()                      override;
    
    void draw();
    
protected: public:
    virtual void onLayoutSubviews(float width, float height);

    virtual void onDrawBackground(float width, float height);
    virtual void onDraw          (float width, float height);
    virtual void onDrawForeground(float width, float height);
    
private:
    CViewController *mViewController  = nullptr;
    bool             mInteractive     = false;
    bool             mVisible         = true;
    MColor::ptr      mBackgroundColor ;
    MRect::ptr       mFrame           ;
    
    MVector<CView::ptr>::ptr mSubviews;
    CView *mSuperview = nullptr;
};
