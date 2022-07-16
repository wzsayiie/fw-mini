#pragma once

#include "cview.h"

class CNavigationController;
class CTabPageController;

m_class(CViewController, CResponder) {
public:
    CViewController();
    
public:
    void makeViewLoad     ();
    void makeViewAppear   ();
    void makeViewDisappear();
    
    void addChildController(const CViewController::ptr &childController);
    void removeFromParentController();
    MVector<CViewController::ptr>::ptr childControllers();
    CViewController::ptr parentController();
    
    void setNavigationController(const std::shared_ptr<CNavigationController> &controller);
    void setTabPageController   (const std::shared_ptr<CTabPageController>    &controller);

    std::shared_ptr<CNavigationController> navigationController();
    std::shared_ptr<CTabPageController>    tabPageController   ();

    bool       viewLoaded  ();
    bool       viewAppeared();
    CView::ptr view        ();
    
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
    std::weak_ptr<CViewController>     mParentController;
    
    std::weak_ptr<CNavigationController> mNavigationController;
    std::weak_ptr<CTabPageController>    mTabPageController   ;

    bool mViewAppeared = false;
    CView::ptr mView;
};
