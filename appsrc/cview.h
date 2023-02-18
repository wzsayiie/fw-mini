#pragma once

#include "clayer.h"
#include "cresponder.h"

class CViewController;

m_class(CView, CResponder) {
public:
    CView();
    CView(float x, float y, float width, float height);
    
public:
    void setViewController (const std::shared_ptr<CViewController> &controller );
    void setInteractive    (bool                                    interactive);
    void setLayouter       (const MFunction<void ()>::ptr          &layer      );
    void setVisible        (bool                                    visible    );
    void setFrame          (const MRect::ptr                       &frame      );
    void setBackgroundColor(const MColor::ptr                      &color      );
    void setAlpha          (float                                   alpha      );

    std::shared_ptr<CViewController> viewController ();
    bool                             interactive    ();
    MFunction<void ()>::ptr          layouter       ();
    CLayer::ptr                      layer          ();
    bool                             visible        ();
    MRect::ptr                       frame          ();
    MRect::ptr                       bounds         ();
    MColor::ptr                      backgroundColor();
    float                            alpha          ();
    
    void animate(double duration, const MFunction<void ()>::ptr &nextAction);
    
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
    
protected: public:
    virtual void onLayoutSubviews(float width, float height);

    virtual void onDraw     (float width, float height);
    virtual void onDrawCover(float width, float height);
    
private:
    void layoutSubviews();

private:
    void on_dispose() override;

private:
    std::weak_ptr<CViewController> mViewController;
    bool                           mInteractive   = false;
    MFunction<void ()>::ptr        mLayouter      ;
    CLayer::ptr                    mLayer         ;
    
    MVector<CView::ptr>::ptr mSubviews ;
    std::weak_ptr<CView>     mSuperview;
};
