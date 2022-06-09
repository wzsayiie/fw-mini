#pragma once

#include "cview.h"

centui_class(CLabel, CView) {
public:
    using upper::upper;

public:
    void setText     (const std::string &text );
    void setTextColor(const MColor::ptr &color);
    void setFontSize (float              size );
    void setHAlign   (MHAlign            align);
    void setVAlign   (MVAlign            align);

    std::string text     ();
    MColor::ptr textColor();
    float       fontSize ();
    MHAlign     hAlign   ();
    MVAlign     vAlign   ();

private:
    void onDraw(float width, float height) override;

private:
    std::string mText      ;
    MColor::ptr mTextColor = MColor::blackColor();
    float       mFontSize  = 16;
    MHAlign     mHAlign    = MHAlign::Center;
    MVAlign     mVAlign    = MVAlign::Middle;
};
