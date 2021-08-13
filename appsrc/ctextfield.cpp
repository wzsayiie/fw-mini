#include "ctextfield.h"

CTextField::CTextField(float x, float y, float w, float h): CControl(x, y, w, h) {
    mCursorTask = m_cast_lambda[this]() {
        mCursorShown = !mCursorShown;
    };
}

CTextField::~CTextField() {
    MCancelTask(mCursorTask.get());
}

void CTextField::setDelegate(CTextFieldDelegate *delegate) {
    mDelegate = delegate;
}

void CTextField::setText(const std::string &text) {
    mText = text;
}

void CTextField::setTextColor(const CColor &color) {
    mTextColor = color;
}

void CTextField::setFontSize(float size) {
    mFontSize = size;
}

const std::string &CTextField::text() {
    return mText;
}

bool CTextField::canRespondText() {
    return true;
}

void CTextField::onTouchEnd(float x, float y) {
    if (0 <= x && x <= width ()
     && 0 <= y && y <= height())
    {
        becomeFirstResponder();
    }
}

void CTextField::onBecomeFirstResponder() {
    //blink the cursor.
    mCursorShown = true;
    MRunEverySeconds(0.6f, mCursorTask.get());

    //enable host text box.
    MStringRef text = m_auto_release MStringCreateU8(mText.c_str());
    MWindowSetTextBoxRawString(text.get());
    MWindowSetTextBoxEnabled(true);
    MWindowSetTextBoxUpdated(true);

    //post the event.
    if (mDelegate) {
        mDelegate->onTextEditBeginEditing(this);
    }
}

void CTextField::onResignFirstResponder() {
    //remove the cursor.
    MCancelTask(mCursorTask.get());
    mCursorShown = false;

    //disable host text box.
    MWindowSetTextBoxEnabled(false);
    MWindowSetTextBoxUpdated(true);

    //post the event.
    if (mDelegate) {
        mDelegate->onTextEditEndEditing(this);
    }
}

void CTextField::onTextInput(const std::string &string, bool enter) {
    if (string != mText) {
        mText = string;

        if (mDelegate) {
            mDelegate->onTextEditTextChange(this);
        }
    }

    if (enter) {
        resignFirstResponder();
    }
}

void CTextField::onDraw(float width, float height) {

    //draw frame box:
    CContextSelectColor(CColor::grayColor);

    const float T = 2;

    CContextDrawRect(0        , 0         , width, T     );
    CContextDrawRect(0        , height - T, width, T     );
    CContextDrawRect(0        , 0         , T    , height);
    CContextDrawRect(width - T, 0         , T    , height);

    //draw text:
    CContextSelectHorizontalAlign(CHorizontalAlign::Left);
    CContextSelectVerticalAlign  (CVerticalAlign::Center);

    CContextSelectString  (mText     );
    CContextSelectFontSize(mFontSize );
    CContextSelectColor   (mTextColor);

    CContextDrawString(0, 0, width, height);

    //draw cursor.
    if (mCursorShown) {
        CContextSelectColor(CColor::blackColor);
        CContextDrawRect(0, (height + mFontSize) / 2, width, 2);
    }
}
