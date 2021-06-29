#include "cview.h"

CView::CView(float x, float y, float width, float height) {
    mX = x;
    mY = y;
    mWidth  = width ;
    mHeight = height;
}

CView::CView() {
}

void CView::setBackgroundColor(CColor color) {
    mBackgroundColor = color;
}

void CView::setOrigin(float x, float y) {
    mX = x;
    mY = y;
}

void CView::setSize(float width, float height) {
    mWidth  = width ;
    mHeight = height;
}

void CView::addSubview(CViewRef subview) {
}

void CView::removeFromSuperview() {
}

const std::vector<CViewRef> &CView::subviews() {
    return mSubviews;
}

CView *CView::superview() {
    return mSuperview;
}

void CView::drawViews() {
}

void CView::drawSize(float width, float height) {
}
