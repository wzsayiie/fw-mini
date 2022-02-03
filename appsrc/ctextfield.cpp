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

    const CColor &FrameColor  = CColor::GrayColor ;
    const CColor &CursorColor = CColor::BlackColor;
    
    const float FW = 2; //frame width.
    const float TI = 4; //text indent.
    const float CH = 2; //cursor height.
    
    //draw frame box:
    CContextSelectColor(FrameColor);

    CContextDrawRect(0         , 0          , width, FW    );
    CContextDrawRect(0         , height - FW, width, FW    );
    CContextDrawRect(0         , 0          , FW   , height);
    CContextDrawRect(width - FW, 0          , FW   , height);

    //draw text:
    CContextSelectHorizontalAlign(CHorizontalAlign::Left);
    CContextSelectVerticalAlign  (CVerticalAlign::Center);

    CContextSelectString  (mText     );
    CContextSelectFontSize(mFontSize );
    CContextSelectColor   (mTextColor);

    CContextDrawString(TI, 0, width - TI * 2, height);

    //draw cursor.
    if (mCursorShown) {
        CContextSelectColor(CursorColor);
        CContextDrawRect(TI, (height + mFontSize) / 2, width - TI * 2, CH);
    }
}
