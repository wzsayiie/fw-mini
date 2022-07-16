#pragma once

#include "ccontrol.h"
#include "clabel.h"

m_class(CButton, CControl) {
public:
    using upper::upper;

public:
    CLabel::ptr titleLabel();
    void setTitle(const std::string &title);
    std::string title();

    void setClickListener(const MFunction<void ()>::ptr &listener);
    MFunction<void ()>::ptr clickListener();

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
    MFunction<void ()>::ptr mClickListener;
    
    bool   mCurrentPointed = false;
    bool   mCurrentTouched = false;
    double mTouchMaskStamp = 0;
};
