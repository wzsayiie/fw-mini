#pragma once

#include "ccontrol.h"
#include "clabel.h"

c_class(CButton, CControl) {
public:
    using upper::upper;

public:
    CLabel::ptr titleLabel();
    void setTitle(const std::string &title);
    std::string title();

    void setClickTarget(const MFunction<void ()>::ptr &target);
    MFunction<void ()>::ptr clickTarget();

protected:
    void onMouseEnter(float x, float y) override;
    void onMouseExit (float x, float y) override;
    void onTouchBegin(float x, float y) override;
    void onTouchEnd  (float x, float y) override;

    void onControlKey(MKey key) override;

    void onLayoutSubviews(float width, float height) override;
    void onDrawForeground(float width, float height) override;

private:
    CLabel::ptr mTitleLabel;
    MFunction<void ()>::ptr mClickTarget;
    
    bool   mCurrentPointed = false;
    bool   mCurrentTouched = false;
    double mTouchMaskStamp = 0;
};
