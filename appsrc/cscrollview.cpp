#include "cscrollview.h"
#include "mcontext.h"
#include "rdefine.h"

//scroll view delegation:

define_reflectable_class_function(CScrollViewDelegation, setScrollingBeginTarget, "setter;args:target")
define_reflectable_class_function(CScrollViewDelegation, setScrollingTarget     , "setter;args:target")
define_reflectable_class_function(CScrollViewDelegation, setScrollingEndTarget  , "setter;args:target")

void CScrollViewDelegation::setScrollingBeginTarget(const MFunction<void ()>::ptr &target) { mScrollingBeginTarget = target; }
void CScrollViewDelegation::setScrollingTarget     (const MFunction<void ()>::ptr &target) { mScrollingTarget      = target; }
void CScrollViewDelegation::setScrollingEndTarget  (const MFunction<void ()>::ptr &target) { mScrollingEndTarget   = target; }

define_reflectable_class_function(CScrollViewDelegation, scrollingBeginTarget, "getter")
define_reflectable_class_function(CScrollViewDelegation, scrollingTarget     , "getter")
define_reflectable_class_function(CScrollViewDelegation, scrollingEndTarget  , "getter")

MFunction<void ()>::ptr CScrollViewDelegation::scrollingBeginTarget() { return mScrollingBeginTarget; }
MFunction<void ()>::ptr CScrollViewDelegation::scrollingTarget     () { return mScrollingTarget     ; }
MFunction<void ()>::ptr CScrollViewDelegation::scrollingEndTarget  () { return mScrollingEndTarget  ; }

define_reflectable_class_function(CScrollViewDelegation, scrollingBegin)
define_reflectable_class_function(CScrollViewDelegation, scrolling     )
define_reflectable_class_function(CScrollViewDelegation, scrollingEnd  )

void CScrollViewDelegation::scrollingBegin() { if (mScrollingBeginTarget) { mScrollingBeginTarget->call(); } }
void CScrollViewDelegation::scrolling     () { if (mScrollingTarget     ) { mScrollingTarget     ->call(); } }
void CScrollViewDelegation::scrollingEnd  () { if (mScrollingEndTarget  ) { mScrollingEndTarget  ->call(); } }

void CScrollViewDelegation::on_dispose() {
    base::dispose();

    mScrollingBeginTarget = nullptr;
    mScrollingTarget      = nullptr;
    mScrollingEndTarget   = nullptr;
}

//assist:

inline float range(float min, float value, float max) {
    return value < min ? min : (max < value ? max : value);
}

const int   IndicatorRGBA  = MColor::LightGrayRGBA & 0xFFffFF80;
const float IndicatorWidth = 4;

//scroll view:

CScrollView::CScrollView(): CScrollView(0, 0, 0, 0) {
}

CScrollView::CScrollView(float x, float y, float width, float height): super(x, y, width, height) {
    setInteractive(true);
}

define_reflectable_class_function(CScrollView, setDelegation, "setter;args:delegation")
void CScrollView::setDelegation(const CScrollViewDelegation::ptr &delegation) {
    mDelegation = delegation;
}

define_reflectable_class_function(CScrollView, delegation, "getter")
CScrollViewDelegation::ptr CScrollView::delegation() {
    if (!mDelegation) {
        mDelegation = CScrollViewDelegation::create();
    }
    return mDelegation;
}

define_reflectable_class_function(CScrollView, addContentSubview, "args:subview")
void CScrollView::addContentSubview(const CView::ptr &subview) {
    contentView()->addSubview(subview);
}

define_reflectable_class_function(CScrollView, setContentOffset, "args:x,y")
void CScrollView::setContentOffset(float x, float y) {
    //adjust content position:
    MRect::ptr content       = contentView()->frame();
    MSize::ptr ownSize       = frame()->size();
    float      contentWidth  = content->width ();
    float      contentHeight = content->height();
    float      ownWidth      = ownSize->width ();
    float      ownHeight     = ownSize->height();

    x = contentWidth  < ownWidth  ? 0 : range(ownWidth  - contentWidth , -x, 0);
    y = contentHeight < ownHeight ? 0 : range(ownHeight - contentHeight, -y, 0);
    
    contentView()->setFrame(MRect::from(x, y, contentWidth, contentHeight));

    //send scrolling event.
    increaseScollingSender();
    sendScrolling();
    reduceScollingSender();
}

define_reflectable_class_function(CScrollView, setContentSize, "args:width,height")
void CScrollView::setContentSize(float width, float height) {
    //adjust content position:
    MRect::ptr content   = contentView()->frame();
    MSize::ptr ownSize   = frame()->size();
    float      contentX  = content->x();
    float      contentY  = content->y();
    float      ownWidth  = ownSize->width ();
    float      ownHeight = ownSize->height();

    float x = width  < ownWidth  ? 0 : range(ownWidth  - width , contentX, 0);
    float y = height < ownHeight ? 0 : range(ownHeight - height, contentY, 0);
    
    contentView()->setFrame(MRect::from(x, y, width, height));

    //send scrolling event.
    increaseScollingSender();
    sendScrolling();
    reduceScollingSender();
}

define_reflectable_class_function(CScrollView, contentView, "getter")
CView::ptr CScrollView::contentView() {
    if (!mContentView) {
        mContentView = CView::create();
        addSubview(mContentView);
    }
    return mContentView;
}

define_reflectable_class_function(CScrollView, contentOffset, "getter")
MPoint::ptr CScrollView::contentOffset() {
    MPoint::ptr origin = contentView()->frame()->origin();
    return MPoint::zero()->sub(origin);
}

define_reflectable_class_function(CScrollView, contentSize, "getter")
MSize::ptr CScrollView::contentSize() {
    return contentView()->frame()->size();
}

void CScrollView::onTouchBegin(float x, float y) {
    mContentTouchX = x - contentView()->frame()->x();
    mContentTouchY = y - contentView()->frame()->y();

    increaseScollingSender();
    sendScrolling();
}

void CScrollView::onTouchMove(float x, float y) {
    setContentOffset(mContentTouchX - x, mContentTouchY - y);

    sendScrolling();
}

void CScrollView::onTouchEnd(float x, float y) {
    setContentOffset(mContentTouchX - x, mContentTouchY - y);

    sendScrolling();
    reduceScollingSender();
}

void CScrollView::onMouseWheel(float x, float y, float delta) {
    MRect::ptr content       = contentView()->frame();
    MSize::ptr ownSize       = frame()->size();
    float      contentX      = content->x();
    float      contentY      = content->y();
    float      contentWidth  = content->width ();
    float      contentHeight = content->height();
    float      ownWidth      = ownSize->width ();
    float      ownHeight     = ownSize->height();

    if /**/ (contentHeight > ownHeight) { contentY += delta; }
    else if (contentWidth  > ownWidth ) { contentX += delta; }

    setContentOffset(-contentX, -contentY);
}

void CScrollView::onLayoutSubviews(float width, float height) {
    MPoint::ptr origin = contentView()->frame()->origin();
    MPoint::ptr offset = MPoint::zero()->sub(origin);
    setContentOffset(offset->x(), offset->y());
}

void CScrollView::onDrawCover(float width, float height) {
    float contentHeight = contentSize()->height();
    float contentWidth  = contentSize()->width ();

    if (contentHeight > height) {
        float offsetY = contentOffset()->y();

        float ix = width - IndicatorWidth;
        float iw = IndicatorWidth;
        float iy = offsetY / contentHeight * height;
        float ih = height  / contentHeight * height;

        MContextSelectRGBA(IndicatorRGBA);
        MContextDrawRect(ix, iy, iw, ih);
    }

    if (contentWidth > width) {
        float offsetX = contentOffset()->x();

        float iy = height - IndicatorWidth;
        float ih = IndicatorWidth;
        float ix = offsetX / contentWidth * width;
        float iw = width   / contentWidth * width;

        MContextSelectRGBA(IndicatorRGBA);
        MContextDrawRect(ix, iy, iw, ih);
    }
}

void CScrollView::increaseScollingSender() {
    mScrollingSenders += 1;
}

void CScrollView::sendScrolling() {
    if (mScrollingSenders <= 0) {
        return;
    }

    MPoint::ptr contentOrigin = contentView()->frame()->origin();
    if (contentOrigin->equal(mLastContentOrigin)) {
        return;
    }

    mLastContentOrigin = contentOrigin;
    if (!mScrollingBegan) {
        mScrollingBegan = true;
        delegation()->scrollingBegin();
    } else {
        delegation()->scrolling();
    }
}

void CScrollView::reduceScollingSender() {
    mScrollingSenders -= 1;

    if (mScrollingSenders == 0 && mScrollingBegan) {
        delegation()->scrollingEnd();
        mScrollingBegan = false;
    }
}

void CScrollView::on_dispose() {
    base::dispose();

    if (mDelegation ) { mDelegation ->dispose(); }
    if (mContentView) { mContentView->dispose(); }
}
