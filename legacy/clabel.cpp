#include "clabel.h"

void CLabel::setText(const std::string &text) {
    mText = text;
}

void CLabel::setTextColor(const CColor &color) {
    mTextColor = color;
}

void CLabel::setFontSize(float size) {
    mFontSize = size;
}

void CLabel::setHorizontalAlign(CHorizontalAlign align) {
    mHorizontalAlign = align;
}

void CLabel::setVerticalAlign(CVerticalAlign align) {
    mVerticalAlign = align;
}

void CLabel::onDraw(float width, float height) {
    if (mText.empty()) {
        return;
    }

    CContextSelectHorizontalAlign(mHorizontalAlign);
    CContextSelectVerticalAlign  (mVerticalAlign  );

    CContextSelectFontSize(mFontSize );
    CContextSelectColor   (mTextColor);
    CContextSelectString  (mText);

    CContextDrawString(0, 0, width, height);
}
