#pragma once

#include "cresponder.h"

c_class(CView, CResponder) {
public:
    CView();
    CView(float x, float y, float width, float height);
    
public: 
    void setViewController (CObject           *controller);
    void setFrame          (const MRect::ptr  &frame     );
    void setBackgroundColor(const MColor::ptr &color     );
    void setVisible        (bool               visible   );
    void setTouchable      (bool               touchable );
    void setAcceptMouseMove(bool               accept    );
    void setAcceptWheel    (bool               accept    );

    CObject    *viewController ();
    MRect::ptr  frame          ();
    MRect::ptr  bounds         ();
    MColor::ptr backgroundColor();
    bool        visible        ();
    bool        touchable      ();
    bool        acceptMouseMove();
    bool        acceptWheel    ();
    
    void addSubview(const CView::ptr &subview);
    void removeFromSuperview();
    MVector<CView::ptr>::ptr subviews();
    CView *superview();
    
    CResponder::ptr findResponder(CResponseEvent event, const MPoint::ptr &pt) override;
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
    bool        mVisible         = true;
    bool        mTouchable       = false;
    bool        mAcceptMouseMove = false;
    bool        mAcceptWheel     = false;
    
    MVector<CView::ptr>::ptr mSubviews;
    CView *mSuperview = nullptr;
};
