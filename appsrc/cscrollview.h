#pragma once

#include "cview.h"

//scroll view delegate:

class CScrollView;
c_class(CScrollViewDelegate, CObject) {
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
    MFunction<void ()>::ptr mScrollingBeginTarget;
    MFunction<void ()>::ptr mScrollingTarget     ;
    MFunction<void ()>::ptr mScrollingEndTarget  ;
};

//scroll view:

c_class(CScrollView, CView) {
public:
    CScrollView();
    CScrollView(float x, float y, float width, float height);

public:
    CScrollViewDelegate::ptr delegate();

    void addContentSubview(const CView::ptr &subview);
    void setContentOffset(float x, float y);
    void setContentSize(float width, float height);

    CView::ptr  contentView  ();
    MPoint::ptr contentOffset();
    MSize::ptr  contentSize  ();

private:
    void increaseScollingSender();
    void sendScrolling();
    void reduceScollingSender();

    void onDrawForeground(float width, float height) override;

    void onTouchBegin(float x, float y) override;
    void onTouchMove (float x, float y) override;
    void onTouchEnd  (float x, float y) override;
    void onWheel     (float delta)      override;

private:
    CScrollViewDelegate::ptr mDelegate;
    CView::ptr mContentView;

    MPoint::ptr mLastContentOrigin;
    int mScrollingSenders = 0;

    float mContentTouchX = 0;
    float mContentTouchY = 0;
};