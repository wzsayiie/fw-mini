#pragma once

#include "ccontrol.h"
#include "clabel.h"

c_class(CButton, CControl) {
public:
    using base::base;

public:
    CLabel::ptr titleLabel();
    void setTitle(const std::string &title);
    std::string title();

    void setClickTarget(const MFunction<void ()>::ptr &target);
    MFunction<void ()>::ptr clickTarget();

private:
    void onMouseEnter(float x, float y) override;
    void onMouseExit (float x, float y) override;
    void onTouchBegin(float x, float y) override;
    void onTouchEnd  (float x, float y) override;

    void onLayoutSubviews(float width, float height) override;
    void onDrawForeground(float width, float height) override;

private:
    CLabel::ptr mTitleLabel;
    MFunction<void ()>::ptr mClickTarget;

    bool mPointed = false;
    bool mTouched = false;
};
