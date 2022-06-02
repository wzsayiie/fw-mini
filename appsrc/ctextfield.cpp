#include "ctextfield.h"

//text field delegate:

define_reflectable_class_function(CTextFieldDelegate, setEditingBeginTarget, "type:set;args:delegate")
define_reflectable_class_function(CTextFieldDelegate, setTextChangeTarget  , "type:set;args:delegate")
define_reflectable_class_function(CTextFieldDelegate, setEditingEndTarget  , "type:set;args:delegate")

void CTextFieldDelegate::setEditingBeginTarget(const MFunction<void ()>::ptr &target) { mEditingBeginTarget = target; }
void CTextFieldDelegate::setTextChangeTarget  (const MFunction<void ()>::ptr &target) { mTextChangeTarget   = target; }
void CTextFieldDelegate::setEditingEndTarget  (const MFunction<void ()>::ptr &target) { mEditingEndTarget   = target; }

define_reflectable_class_function(CTextFieldDelegate, editingBeginTarget, "type:get")
define_reflectable_class_function(CTextFieldDelegate, textChangeTarget  , "type:get")
define_reflectable_class_function(CTextFieldDelegate, editingEndTarget  , "type:get")

MFunction<void ()>::ptr CTextFieldDelegate::editingBeginTarget() { return mEditingBeginTarget; }
MFunction<void ()>::ptr CTextFieldDelegate::textChangeTarget  () { return mTextChangeTarget  ; }
MFunction<void ()>::ptr CTextFieldDelegate::editingEndTarget  () { return mEditingEndTarget  ; }

define_reflectable_class_function(CTextFieldDelegate, editingBegin)
define_reflectable_class_function(CTextFieldDelegate, textChange  )
define_reflectable_class_function(CTextFieldDelegate, editingEnd  )

void CTextFieldDelegate::editingBegin() { if (mEditingBeginTarget) { mEditingBeginTarget->call(); } }
void CTextFieldDelegate::textChange  () { if (mTextChangeTarget  ) { mTextChangeTarget  ->call(); } }
void CTextFieldDelegate::editingEnd  () { if (mEditingEndTarget  ) { mEditingEndTarget  ->call(); } }

//assist:

const float CursorHeight   = 4.0f;
const float CursorInterval = 0.6f;

//text field:

define_reflectable_class_function(CTextField, setDelegate, "type:set;args:delegate")
void CTextField::setDelegate(const CTextFieldDelegate::ptr &delegate) {
    mDelegate = delegate;
}

define_reflectable_class_function(CTextField, delegate, "type:get")
CTextFieldDelegate::ptr CTextField::delegate() {
    if (!mDelegate) {
        mDelegate = CTextFieldDelegate::create();
    }
    return mDelegate;
}

define_reflectable_class_function(CTextField, setText, "type:set;args:text")
void CTextField::setText(const std::string &text) {
    mText = text;

    increaseEditingSender();
    sendEditing();
    reduceEditingSender();
}

define_reflectable_class_function(CTextField, setTextColor, "type:set;args:color")
define_reflectable_class_function(CTextField, setFontSize , "type:set;args:size" )
define_reflectable_class_function(CTextField, setHAlign   , "type:set;args:align")
define_reflectable_class_function(CTextField, setVAlign   , "type:set;args:align")

void CTextField::setTextColor(const MColor::ptr &color) { mTextColor = color; }
void CTextField::setFontSize (float              size ) { mFontSize  = size ; }
void CTextField::setHAlign   (MHAlign            align) { mHAlign    = align; }
void CTextField::setVAlign   (MVAlign            align) { mVAlign    = align; }

define_reflectable_class_function(CTextField, text     , "type:get")
define_reflectable_class_function(CTextField, entered  , "type:get")
define_reflectable_class_function(CTextField, textColor, "type:get")
define_reflectable_class_function(CTextField, fontSize , "type:get")
define_reflectable_class_function(CTextField, hAlign   , "type:get")
define_reflectable_class_function(CTextField, vAlign   , "type:get")

std::string CTextField::text     () { return mText     ; }
bool        CTextField::entered  () { return mEntered  ; }
MColor::ptr CTextField::textColor() { return mTextColor ? mTextColor : MColor::clearColor(); }
float       CTextField::fontSize () { return mFontSize ; }
MHAlign     CTextField::hAlign   () { return mHAlign   ; }
MVAlign     CTextField::vAlign   () { return mVAlign   ; }

void CTextField::increaseEditingSender() {
    mEditingSenders += 1;
}

void CTextField::sendEditing() {
    if (mEditingSenders <= 0) {
        return;
    }
    if (mLastText == mText && mLastEntered == mEntered) {
        return;
    }

    mLastText = mText;
    mLastEntered = mEntered;

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

bool CTextField::canRespondWriting(const MPoint::ptr &pt) {
    return true;
}

void CTextField::onWrite(const std::string &string, bool enter) {
    mText = string;
    mEntered = enter;

    sendEditing();
}

void CTextField::onDraw(float width, float height) {
    if (!mTextColor || mTextColor->none()) {
        return;
    }

    //draw cursor.
    if (mCursorBeginTick) {
        double current  = MScheduler::instance()->GetSecondsTick();
        double duration = current - mCursorBeginTick;
        auto   gapCount = (int64_t)(duration * 1000) / (int64_t)(CursorInterval * 1000);

        if (gapCount % 2) {
            MContextSelectRGBA(mTextColor->rgba());
            MContextDrawRect(0, height - CursorHeight, width, CursorHeight);
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
