#pragma once

#include "cview.h"

//scroll view delegation:

m_class(CScrollViewDelegation, CObject) {
public:
    void setScrollingBeginTarget(const MFunction<void ()>::ptr &target);
    void setScrollingTarget     (const MFunction<void ()>::ptr &target);
    void setScrollingEndTarget  (const MFunction<void ()>::ptr &target);

    MFunction<void ()>::ptr scrollingBeginTarget();
    MFunction<void ()>::ptr scrollingTarget     ();
    MFunction<void ()>::ptr scrollingEndTarget  ();

    void scrollingBegin();
    void scrolling     ();
    void scrollingEnd  ();

private:
    void on_dispose() override;

private:
    MFunction<void ()>::ptr mScrollingBeginTarget;
    MFunction<void ()>::ptr mScrollingTarget     ;
    MFunction<void ()>::ptr mScrollingEndTarget  ;
};

//scroll view:

m_class(CScrollView, CView) {
public:
    CScrollView();
    CScrollView(float x, float y, float width, float height);

public:
    void setDelegation(const CScrollViewDelegation::ptr &delegation);
    CScrollViewDelegation::ptr delegation();

    void addContentSubview(const CView::ptr &subview);
    void setContentOffset(float x, float y);
    void setContentSize(float width, float height);

    CView::ptr  contentView  ();
    MPoint::ptr contentOffset();
    MSize::ptr  contentSize  ();

protected:
    void onTouchBegin(float x, float y)              override;
    void onTouchMove (float x, float y)              override;
    void onTouchEnd  (float x, float y)              override;
    void onMouseWheel(float x, float y, float delta) override;

    void onLayoutSubviews(float width, float height) override;
    void onDrawCover     (float width, float height) override;

private:
    void increaseScollingSender();
    void sendScrolling();
    void reduceScollingSender();

private:
    void on_dispose() override;

private:
    CScrollViewDelegation::ptr mDelegation;
    CView::ptr mContentView;

    MPoint::ptr mLastContentOrigin;
    bool mScrollingBegan   = false;
    int  mScrollingSenders = 0;

    float mContentTouchX = 0;
    float mContentTouchY = 0;
};
