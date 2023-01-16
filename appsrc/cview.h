#pragma once

#include "cresponder.h"
#include "mgeneric.h"
#include "mgraphics.h"

class CViewController;

m_class(CView, CResponder) {
public:
    CView();
    CView(float x, float y, float width, float height);
    
public:
    void setViewController  (const std::shared_ptr<CViewController> &controller );
    void setInteractive     (bool                                    interactive);
    void setVisible         (bool                                    visible    );
    void setBackgroundColor (const MColor::ptr                      &color      );
    void setLayoutDelegation(const MFunction<void ()>::ptr          &delegation );
    void setFrame           (const MRect::ptr                       &frame      );

    std::shared_ptr<CViewController> viewController  ();
    bool                             interactive     ();
    bool                             visible         ();
    MColor::ptr                      backgroundColor ();
    MFunction<void ()>::ptr          layoutDelegation();
    MRect::ptr                       bounds          ();
    MRect::ptr                       frame           ();
    
    void addSubview(const CView::ptr &subview);
    void removeFromSuperview();
    MVector<CView::ptr>::ptr subviews();
    CView::ptr superview();
    
    CResponder::ptr findResponder (MEvent evt, const MPoint::ptr &pt) override;
    bool            existResponder(const CResponder::ptr &responder) override;
    MPoint::ptr     responseOffset() override;
    
    bool canBecomeFocusResponder() override;

    bool canRespondTouch     (const MPoint::ptr &pt) override;
    bool canRespondMouseMove (const MPoint::ptr &pt) override;
    bool canRespondMouseWheel(const MPoint::ptr &pt) override;
    bool canRespondKbKey     ()                      override;
    bool canRespondWriting   ()                      override;
    
    void draw();
    
protected: public:
    virtual void onLayoutSubviews(float width, float height);

    virtual void onDrawBackground(float width, float height);
    virtual void onDraw          (float width, float height);
    virtual void onDrawForeground(float width, float height);
    
private:
    void layoutSubviews();

private:
    void on_dispose() override;

private:
    std::weak_ptr<CViewController> mViewController  ;
    bool                           mInteractive     = false;
    bool                           mVisible         = true;
    MColor::ptr                    mBackgroundColor ;
    MFunction<void ()>::ptr        mLayoutDelegation;
    MRect::ptr                     mFrame           ;
    
    MVector<CView::ptr>::ptr mSubviews ;
    std::weak_ptr<CView>     mSuperview;
};
