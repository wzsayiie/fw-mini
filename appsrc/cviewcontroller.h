#pragma once

#include "cview.h"

class CViewController;
declare_reflectable_special(MVector<std::shared_ptr<CViewController>>)

declare_reflectable_class(CViewController)
class CViewController : public CExtends<CViewController, CResponder> {
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
    
    CResponder::ptr findResponder(float x, float y, const CResponderDetector::ptr &fit) override;
    MPoint::ptr responseOffset() override;
    
    bool canRespondTouch    (float x, float y) override;
    bool canRespondMouseMove(float x, float y) override;
    bool canRespondWriting  (float x, float y) override;
    bool canRespondKey      (float x, float y) override;
    bool canRespondWheel    (float x, float y) override;
    
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
