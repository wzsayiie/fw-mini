#include "cscrollview.h"

//scroll view delegate:

define_reflectable_class_function(CScrollViewDelegate, setScrollingBeginTarget, "args:target")
define_reflectable_class_function(CScrollViewDelegate, setScrollingTarget     , "args:target")
define_reflectable_class_function(CScrollViewDelegate, setScrollingEndTarget  , "args:target")

void CScrollViewDelegate::setScrollingBeginTarget(const MFunction<void ()>::ptr &target) { mScrollingBeginTarget = target; }
void CScrollViewDelegate::setScrollingTarget     (const MFunction<void ()>::ptr &target) { mScrollingTarget      = target; }
void CScrollViewDelegate::setScrollingEndTarget  (const MFunction<void ()>::ptr &target) { mScrollingEndTarget   = target; }

define_reflectable_class_function(CScrollViewDelegate, scrollingBeginTarget)
define_reflectable_class_function(CScrollViewDelegate, scrollingTarget     )
define_reflectable_class_function(CScrollViewDelegate, scrollingEndTarget  )

MFunction<void ()>::ptr CScrollViewDelegate::scrollingBeginTarget() { return mScrollingBeginTarget; }
MFunction<void ()>::ptr CScrollViewDelegate::scrollingTarget     () { return mScrollingTarget     ; }
MFunction<void ()>::ptr CScrollViewDelegate::scrollingEndTarget  () { return mScrollingEndTarget  ; }

define_reflectable_class_function(CScrollViewDelegate, scrollingBegin)
define_reflectable_class_function(CScrollViewDelegate, scrolling     )
define_reflectable_class_function(CScrollViewDelegate, scrollingEnd  )

void CScrollViewDelegate::scrollingBegin() { if (mScrollingBeginTarget) { mScrollingBeginTarget->call(); } }
void CScrollViewDelegate::scrolling     () { if (mScrollingTarget     ) { mScrollingTarget     ->call(); } }
void CScrollViewDelegate::scrollingEnd  () { if (mScrollingEndTarget  ) { mScrollingEndTarget  ->call(); } }

//assist:

inline float range(float min, float value, float max) {
    return value < min ? min : (max < value ? max : value);
}

//scroll view:

const int   IndicatorRGBA  = MColor::LightGrayRGBA & 0xFFffFF80;
const float IndicatorWidth = 4;

CScrollView::CScrollView() {
    setAcceptWheel(true);
    setTouchable(true);
}

CScrollView::CScrollView(float x, float y, float width, float height): CScrollView() {
    setFrame(MRect::from(x, y, width, height));
}

define_reflectable_class_function(CScrollView, delegate)
CScrollViewDelegate::ptr CScrollView::delegate() {
    if (!mDelegate) {
        mDelegate = CScrollViewDelegate::create();
    }
    return mDelegate;
}

define_reflectable_class_function(CScrollView, addContentSubview, "args:subview")
void CScrollView::addContentSubview(const CView::ptr &subview) {
    contentView()->addSubview(subview);
}

define_reflectable_class_function(CScrollView, setContentOffset, "args:offset")
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

define_reflectable_class_function(CScrollView, setContentSize, "args:size")
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

define_reflectable_class_function(CScrollView, contentView)
CView::ptr CScrollView::contentView() {
    if (!mContentView) {
        mContentView = CView::create();
        addSubview(mContentView);
    }
    return mContentView;
}

define_reflectable_class_function(CScrollView, contentOffset)
MPoint::ptr CScrollView::contentOffset() {
    MPoint::ptr origin = contentView()->frame()->origin();
    return MPoint::zero()->sub(origin);
}

define_reflectable_class_function(CScrollView, contentSize)
MSize::ptr CScrollView::contentSize() {
    return contentView()->frame()->size();
}

void CScrollView::increaseScollingSender() {
    mScrollingSenders += 1;

    if (mScrollingSenders == 1) {
        delegate()->scrollingBegin();
    }
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
    delegate()->scrolling();
}

void CScrollView::reduceScollingSender() {
    mScrollingSenders -= 1;

    if (mScrollingSenders == 0) {
        delegate()->scrollingEnd();
    }
}

void CScrollView::onDrawForeground(float width, float height) {
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

void CScrollView::onWheel(float delta) {
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