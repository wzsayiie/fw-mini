#pragma once

#include "ccontrol.h"

class CTextField;

//text field delegate:

struct CTextFieldDelegate {
    virtual ~CTextFieldDelegate() {}

    virtual void onTextEditBeginEditing(CTextField *textField) {}
    virtual void onTextEditTextChange  (CTextField *textField) {}
    virtual void onTextEditEndEditing  (CTextField *textField) {}
};

//text field:

def_class(CTextField) : public CControl {

public:
    CTextField(float x, float y, float width, float height);
    ~CTextField();

    void setDelegate(CTextFieldDelegate *delegate);

    void setText(const std::string &text);
    void setTextColor(const CColor &color);
    void setFontSize(float size);

    const std::string &text();

    bool canRespondText() override;

protected:
    void onTouchEnd(float x, float y) override;

    void onBecomeFirstResponder() override;
    void onResignFirstResponder() override;

    void onTextInput(const std::string &string, bool enter) override;

    void onDraw(float width, float height) override;

private:
    CTextFieldDelegate *mDelegate = nullptr;

    MLambdaRef  mCursorTask;
    bool        mCursorShown = false;
    std::string mText;
    CColor      mTextColor {0, 0, 0};
    float       mFontSize = 16;
};
