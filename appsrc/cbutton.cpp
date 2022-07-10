#include "cbutton.h"

define_reflectable_class_function(CButton, titleLabel, "getter")
CLabel::ptr CButton::titleLabel() {
    if (!mTitleLabel) {
        mTitleLabel = CLabel::create();
        mTitleLabel->setFrame(bounds());
        addSubview(mTitleLabel);
    }
    return mTitleLabel;
}

define_reflectable_class_function(CButton, setTitle, "setter;args:title")
void CButton::setTitle(const std::string &title) {
    titleLabel()->setText(title);
}

define_reflectable_class_function(CButton, title)
std::string CButton::title() {
    return titleLabel()->text();
}

define_reflectable_class_function(CButton, setClickTarget, "setter;args:target")
void CButton::setClickTarget(const MFunction<void ()>::ptr &target) {
    mClickTarget = target;
}

define_reflectable_class_function(CButton, clickTarget, "getter")
MFunction<void ()>::ptr CButton::clickTarget() {
    return mClickTarget;
}

void CButton::onMouseEnter(float x, float y) { mPointed = true ; }
void CButton::onMouseExit (float x, float y) { mPointed = false; }
void CButton::onTouchBegin(float x, float y) { mTouched = true ; }

void CButton::onTouchEnd(float x, float y) {
    mTouched = false;

    if (mClickTarget && bounds()->contains(MPoint::from(x, y))) {
        mClickTarget->call();
    }
}

void CButton::onLayoutSubviews(float width, float height) {
    titleLabel()->setFrame(MRect::from(0, 0, width, height));
}

void CButton::onDrawForeground(float width, float height) {
    //NOTE: reserve focus frame drawn by control.
    base::onDrawForeground(width, height);
    
    if (mTouched) {
        MContextSelectRGBA(0xFFffFF80 & MColor::LightGrayRGBA);
        MContextDrawRect(0, 0, width, height);
    } else if (mPointed) {
        MContextSelectRGBA(0xFFffFF80 & MColor::GrayRGBA);
        MContextDrawRect(0, 0, width, height);
    }
}
