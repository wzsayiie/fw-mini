#include "ctextfield.h"

//text field delegate:

define_reflectable_class_function(CTextFieldDelegate, setEditingBeginTarget, "setter;args:delegate")
define_reflectable_class_function(CTextFieldDelegate, setTextChangeTarget  , "setter;args:delegate")
define_reflectable_class_function(CTextFieldDelegate, setEditingEndTarget  , "setter;args:delegate")

void CTextFieldDelegate::setEditingBeginTarget(const MFunction<void ()>::ptr &target) { mEditingBeginTarget = target; }
void CTextFieldDelegate::setTextChangeTarget  (const MFunction<void ()>::ptr &target) { mTextChangeTarget   = target; }
void CTextFieldDelegate::setEditingEndTarget  (const MFunction<void ()>::ptr &target) { mEditingEndTarget   = target; }

define_reflectable_class_function(CTextFieldDelegate, editingBeginTarget, "getter")
define_reflectable_class_function(CTextFieldDelegate, textChangeTarget  , "getter")
define_reflectable_class_function(CTextFieldDelegate, editingEndTarget  , "getter")

MFunction<void ()>::ptr CTextFieldDelegate::editingBeginTarget() { return mEditingBeginTarget; }
MFunction<void ()>::ptr CTextFieldDelegate::textChangeTarget  () { return mTextChangeTarget  ; }
MFunction<void ()>::ptr CTextFieldDelegate::editingEndTarget  () { return mEditingEndTarget  ; }

define_reflectable_class_function(CTextFieldDelegate, editingBegin)
define_reflectable_class_function(CTextFieldDelegate, textChange  )
define_reflectable_class_function(CTextFieldDelegate, editingEnd  )

void CTextFieldDelegate::editingBegin() { if (mEditingBeginTarget) { mEditingBeginTarget->call(); } }
void CTextFieldDelegate::textChange  () { if (mTextChangeTarget  ) { mTextChangeTarget  ->call(); } }
void CTextFieldDelegate::editingEnd  () { if (mEditingEndTarget  ) { mEditingEndTarget  ->call(); } }

//text field:

define_reflectable_class_function(CTextField, setDelegate, "setter;args:delegate")
void CTextField::setDelegate(const CTextFieldDelegate::ptr &delegate) {
    mDelegate = delegate;
}

define_reflectable_class_function(CTextField, delegate, "getter")
CTextFieldDelegate::ptr CTextField::delegate() {
    if (!mDelegate) {
        mDelegate = CTextFieldDelegate::create();
    }
    return mDelegate;
}

define_reflectable_class_function(CTextField, setText, "setter;args:text")
void CTextField::setText(const std::string &text) {
    mText = text;

    increaseEditingSender();
    sendEditing();
    reduceEditingSender();
}

define_reflectable_class_function(CTextField, setTextColor, "setter;args:color")
define_reflectable_class_function(CTextField, setFontSize , "setter;args:size" )
define_reflectable_class_function(CTextField, setHAlign   , "setter;args:align")
define_reflectable_class_function(CTextField, setVAlign   , "setter;args:align")

void CTextField::setTextColor(const MColor::ptr &color) { mTextColor = color; }
void CTextField::setFontSize (float              size ) { mFontSize  = size ; }
void CTextField::setHAlign   (MHAlign            align) { mHAlign    = align; }
void CTextField::setVAlign   (MVAlign            align) { mVAlign    = align; }

define_reflectable_class_function(CTextField, text     , "getter")
define_reflectable_class_function(CTextField, textColor, "getter")
define_reflectable_class_function(CTextField, fontSize , "getter")
define_reflectable_class_function(CTextField, hAlign   , "getter")
define_reflectable_class_function(CTextField, vAlign   , "getter")

std::string CTextField::text     () { return mText     ; }
MColor::ptr CTextField::textColor() { return mTextColor ? mTextColor : MColor::clearColor(); }
float       CTextField::fontSize () { return mFontSize ; }
MHAlign     CTextField::hAlign   () { return mHAlign   ; }
MVAlign     CTextField::vAlign   () { return mVAlign   ; }

void CTextField::onBecomeFocusResponder() {
    increaseEditingSender();

    MWindow::mainWindow()->setWritingEnabled(true, mText);
    mCursorBeginTick = MScheduler::instance()->GetSecondsTick();
}

void CTextField::onResignFocusResponder() {
    reduceEditingSender();

    MWindow::mainWindow()->setWritingEnabled(false, "");
    mCursorBeginTick = 0;
}

bool CTextField::canRespondWriting() {
    return true;
}

void CTextField::onWrite(const std::string &string, bool enter) {
    mText = string;
    sendEditing();

    //press enter to end editing.
    if (enter) {
        transferFocusControl();
    }
}

void CTextField::onDrawForeground(float width, float height) {
    //block the focus frame of the focused control.
}

void CTextField::onDraw(float width, float height) {
    if (!mTextColor || mTextColor->none()) {
        return;
    }

    //draw cursor.
    float interval = 0.6f;
    float thick    = 2.0f;

    if (mCursorBeginTick) {
        double now      = MScheduler::instance()->GetSecondsTick();
        double duration = now - mCursorBeginTick;
        auto   count    = (int64_t)(duration * 1000) / (int64_t)(interval * 1000);

        if (count % 2) {
            MContextSelectRGBA(mTextColor->rgba());

            MContextDrawRect(0,              0, width, thick ); //top.
            MContextDrawRect(0, height - thick, width, thick ); //bottom.
            MContextDrawRect(0,              0, thick, height); //left.
            MContextDrawRect(width - thick,  0, thick, height); //right.
        }
    }

    //draw text.
    if (!mText.empty() && mFontSize > 0) {

        MContextSelectText    (mText    );
        MContextSelectRGBA    (mTextColor->rgba());
        MContextSelectFontSize(mFontSize);
        MContextSelectHAlign  (mHAlign  );
        MContextSelectVAlign  (mVAlign  );

        MContextDrawText(0, 0, width, height);
    }
}

void CTextField::increaseEditingSender() {
    mEditingSenders += 1;
}

void CTextField::sendEditing() {
    if (mEditingSenders <= 0) {
        return;
    }
    if (mLastText == mText) {
        return;
    }

    mLastText = mText;

    //NOTE: firstly emit "editing begin".
    if (!mEditingBegan) {
        mEditingBegan = true;
        delegate()->editingBegin();
    }

    delegate()->textChange();
}

void CTextField::reduceEditingSender() {
    mEditingSenders -= 1;

    if (mEditingSenders == 0 && mEditingBegan) {
        delegate()->editingEnd();
        mEditingBegan = false;
    }
}
