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

define_reflectable_class_function(CButton, setClickListener, "setter;args:listener")
void CButton::setClickListener(const MFunction<void ()>::ptr &listener) {
    mClickListener = listener;
}

define_reflectable_class_function(CButton, clickListener, "getter")
MFunction<void ()>::ptr CButton::clickListener() {
    return mClickListener;
}

void CButton::onMouseEnter(float x, float y) { mCurrentPointed = true ; }
void CButton::onMouseExit (float x, float y) { mCurrentPointed = false; }
void CButton::onTouchBegin(float x, float y) { mCurrentTouched = true ; }

void CButton::onTouchEnd(float x, float y) {
    mCurrentTouched = false;

    if (mClickListener && bounds()->contains(MPoint::from(x, y))) {
        mClickListener->call();
    }
}

void CButton::onControlKey(MKey key) {
    if (key != MKey::Enter) {
        return;
    }

    double now = MScheduler::instance()->GetSecondsTick();
    mTouchMaskStamp = now + 0.1;

    if (mClickListener) {
        mClickListener->call();
    }
}

void CButton::onLayoutSubviews(float width, float height) {
    titleLabel()->setFrame(MRect::from(0, 0, width, height));
}

void CButton::onDrawForeground(float width, float height) {
    //NOTE: reserve focus frame drawn by control.
    base::onDrawForeground(width, height);
    
    double now = MScheduler::instance()->GetSecondsTick();
    if (mCurrentTouched || now < mTouchMaskStamp) {
        //touched by mouse or keyboard.
        MContextSelectRGBA(0xFFffFF80 & MColor::LightGrayRGBA);
        MContextDrawRect(0, 0, width, height);

    } else if (mCurrentPointed) {
        //pointed by mouse.
        MContextSelectRGBA(0xFFffFF80 & MColor::GrayRGBA);
        MContextDrawRect(0, 0, width, height);
    }
}
