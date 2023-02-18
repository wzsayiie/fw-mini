#pragma once

#include "ccontrol.h"
#include "clabel.h"

m_class(CButton, CControl) {
public:
    using super::super;

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

    void onControlKbKey(MKbKeyCode code) override;

    void onLayoutSubviews(float width, float height) override;
    void onDrawCover     (float width, float height) override;

private:
    void on_dispose() override;

private:
    CLabel::ptr mTitleLabel;
    MFunction<void ()>::ptr mClickTarget;
    
    bool   mCurrentPointed = false;
    bool   mCurrentTouched = false;
    double mTouchMaskStamp = 0;
};
