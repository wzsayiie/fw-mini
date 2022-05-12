#pragma once

#include "ccontrol.h"

//text field delegate:

class CTextField;
c_class(CTextFieldDelegate, CObject) {
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
    MFunction<void ()>::ptr mEditingBeginTarget;
    MFunction<void ()>::ptr mTextChangeTarget  ;
    MFunction<void ()>::ptr mEditingEndTarget  ;
};

//text field:

c_class(CTextField, CControl) {
public:
    using base::base;

public:
    void setDelegate(const CTextFieldDelegate::ptr &delegate);
    CTextFieldDelegate::ptr delegate();

    void setText     (const std::string &text );
    void setTextColor(const MColor::ptr &color);
    void setFontSize (float              size );
    void setHAlign   (MHAlign            align);
    void setVAlign   (MVAlign            align);

    std::string text     ();
    bool        entered  ();
    MColor::ptr textColor();
    float       fontSize ();
    MHAlign     hAlign   ();
    MVAlign     vAlign   ();

private:
    void increaseEditingSender();
    void sendEditing();
    void reduceEditingSender();

    void onBecomeFocusResponder() override;
    void onResignFocusResponder() override;

    bool canRespondWriting(const MPoint::ptr &pt) override;
    void onWrite(const std::string &string, bool enter) override;

    void onDraw(float width, float height) override;

private:
    CTextFieldDelegate::ptr mDelegate;
    
    std::string mText;
    bool        mEntered   = false;
    MColor::ptr mTextColor = MColor::blackColor();
    float       mFontSize  = 16;
    MHAlign     mHAlign    = MHAlign::Left;
    MVAlign     mVAlign    = MVAlign::Middle;

    std::string mLastText;
    bool        mLastEntered     = false;
    int         mEditingSenders  = 0;
    bool        mEditingBegan    = false;
    double      mCursorBeginTick = 0;
};
