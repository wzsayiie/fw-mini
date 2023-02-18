#pragma once

#include "ccontrol.h"

//text field delegation:

m_class(CTextFieldDelegation, CObject) {
public:
    void setEditingBeginTarget(const MFunction<void ()>::ptr &target);
    void setTextChangeTarget  (const MFunction<void ()>::ptr &target);
    void setEditingEndTarget  (const MFunction<void ()>::ptr &target);

    MFunction<void ()>::ptr editingBeginTarget();
    MFunction<void ()>::ptr textChangeTarget  ();
    MFunction<void ()>::ptr editingEndTarget  ();

    void editingBegin();
    void textChange  ();
    void editingEnd  ();

private:
    void on_dispose() override;

private:
    MFunction<void ()>::ptr mEditingBeginTarget;
    MFunction<void ()>::ptr mTextChangeTarget  ;
    MFunction<void ()>::ptr mEditingEndTarget  ;
};

//text field:

m_class(CTextField, CControl) {
public:
    using super::super;

public:
    void setDelegation(const CTextFieldDelegation::ptr &delegation);
    CTextFieldDelegation::ptr delegation();

    void setText     (const std::string &text  );
    void setPrompt   (const std::string &prompt);
    void setTextColor(const MColor::ptr &color );
    void setFontSize (float              size  );
    void setHAlign   (MHAlign            align );
    void setVAlign   (MVAlign            align );

    std::string text     ();
    std::string prompt   ();
    MColor::ptr textColor();
    float       fontSize ();
    MHAlign     hAlign   ();
    MVAlign     vAlign   ();
    bool        entered  ();

protected:
    void onBecomeFocusResponder() override;
    void onResignFocusResponder() override;

    void onWriting     (const std::string &text) override;
    void onControlKbKey(MKbKeyCode         code) override;

    void onDrawCover(float width, float height) override;
    void onDraw     (float width, float height) override;

private:
    void increaseEditingSender();
    void sendEditing();
    void reduceEditingSender();

private:
    void on_dispose() override;

private:
    CTextFieldDelegation::ptr mDelegation;
    
    std::string mText      ;
    std::string mPrompt    ;
    MColor::ptr mTextColor = MColor::blackColor();
    float       mFontSize  = 16;
    MHAlign     mHAlign    = MHAlign::Left;
    MVAlign     mVAlign    = MVAlign::Middle;
    bool        mEntered   = false;

    std::string mLastText        ;
    int         mEditingSenders  = 0;
    double      mCursorBeginTick = 0;
};
