#pragma once

#include "cview.h"

c_class(CViewController, CResponder) {
public:
    CViewController();
    
public:
    void makeViewLoad     ();
    void makeViewAppear   ();
    void makeViewDisappear();
    
    void addChildController(const CViewController::ptr &childController);
    void removeFromParentController();
    MVector<CViewController::ptr>::ptr childControllers();
    CViewController *parentController();
    
    CView::ptr view();
    
    CResponder::ptr findResponder (CResponseEvent event, const MPoint::ptr &pt) override;
    bool            existResponder(const CResponder::ptr &responder) override;
    MPoint::ptr     responseOffset() override;
    
    bool canBecomeFocusResponder() override;

    bool canRespondTouch    (const MPoint::ptr &pt) override;
    bool canRespondMouseMove(const MPoint::ptr &pt) override;
    bool canRespondWheel    (const MPoint::ptr &pt) override;
    bool canRespondKey      ()                      override;
    bool canRespondWriting  ()                      override;
    
protected: public:
    virtual CView::ptr loadView();
    
    virtual void onViewLoad     ();
    virtual void onViewAppear   ();
    virtual void onViewDisappear();
    
private:
    MVector<CViewController::ptr>::ptr mChildControllers;
    CViewController *mParentController = nullptr;
    
    bool mViewLoaded   = false;
    bool mViewAppeared = false;
    
    CView::ptr mView;
};
