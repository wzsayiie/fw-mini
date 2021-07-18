#pragma once

#include <string>
#include "cview.h"

typedef std::shared_ptr<class CLabel> CLabelRef;

class CLabel : public CView {

public:
    using CView::CView;

    void setText(const std::string &text);
    void setTextColor(const CColor &color);
    void setFontSize(float size);

    void setHorizontalAlign(CHorizontalAlign align);
    void setVerticalAlign  (CVerticalAlign   align);

protected:
    void onDraw(float width, float height) override;

private:
    CHorizontalAlign mHorizontalAlign = CHorizontalAlign::Center;
    CVerticalAlign   mVerticalAlign   = CVerticalAlign  ::Center;

    std::string mText;
    CColor      mTextColor {0, 0, 0};
    float       mFontSize = 16;
};
