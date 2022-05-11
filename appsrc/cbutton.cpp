#include "cbutton.h"

const int PointedForegroundRGBA = MColor::LightGrayRGBA & 0xFFffFF80;
const int TouchedForegroundRGBA = MColor::GrayRGBA      & 0xFFffFF80;

define_reflectable_class_function(CButton, titleLabel)
CLabel::ptr CButton::titleLabel() {
    if (!mTitleLabel) {
        mTitleLabel = CLabel::create();
        mTitleLabel->setFrame(bounds());
        addSubview(mTitleLabel);
    }
    return mTitleLabel;
}

define_reflectable_class_function(CButton, setTitle, "args:title")
void CButton::setTitle(const std::string &title) {
    titleLabel()->setText(title);
}

define_reflectable_class_function(CButton, title)
std::string CButton::title() {
    return titleLabel()->text();
}

define_reflectable_class_function(CButton, setClickTarget, "args:target")
void CButton::setClickTarget(const MFunction<void ()>::ptr &target) {
    mClickTarget = target;
}

define_reflectable_class_function(CButton, clickTarget)
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
    if (mTouched) {
        MContextSelectRGBA(TouchedForegroundRGBA);
        MContextDrawRect(0, 0, width, height);
    } else if (mPointed) {
        MContextSelectRGBA(PointedForegroundRGBA);
        MContextDrawRect(0, 0, width, height);
    }
}
