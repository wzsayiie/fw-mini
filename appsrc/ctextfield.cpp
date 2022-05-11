#include "ctextfield.h"
#include <ctime>

//text field delegate:

define_reflectable_class_function(CTextFieldDelegate, setEditingBeginTarget, "args:delegate")
define_reflectable_class_function(CTextFieldDelegate, setTextChangeTarget  , "args:delegate")
define_reflectable_class_function(CTextFieldDelegate, setEditingEndTarget  , "args:delegate")

void CTextFieldDelegate::setEditingBeginTarget(const MFunction<void ()>::ptr &target) { mEditingBeginTarget = target; }
void CTextFieldDelegate::setTextChangeTarget  (const MFunction<void ()>::ptr &target) { mTextChangeTarget   = target; }
void CTextFieldDelegate::setEditingEndTarget  (const MFunction<void ()>::ptr &target) { mEditingEndTarget   = target; }

define_reflectable_class_function(CTextFieldDelegate, editingBeginTarget)
define_reflectable_class_function(CTextFieldDelegate, textChangeTarget  )
define_reflectable_class_function(CTextFieldDelegate, editingEndTarget  )

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

inline float GetNowTick() {
    return (float)clock() / CLOCKS_PER_SEC;
}

const float CursorHeight   = 4.0f;
const float CursorInterval = 0.6f;

//text field:

define_reflectable_class_function(CTextField, setDelegate, "args:delegate")
void CTextField::setDelegate(const CTextFieldDelegate::ptr &delegate) {
    mDelegate = delegate;
}

define_reflectable_class_function(CTextField, delegate)
CTextFieldDelegate::ptr CTextField::delegate() {
    if (!mDelegate) {
        mDelegate = CTextFieldDelegate::create();
    }
    return mDelegate;
}

define_reflectable_class_function(CTextField, setText, "args:text")
void CTextField::setText(const std::string &text) {
    mText = text;

    increaseEditingSender();
    sendEditing();
    reduceEditingSender();
}

define_reflectable_class_function(CTextField, setTextColor, "args:color")
define_reflectable_class_function(CTextField, setFontSize , "args:size" )
define_reflectable_class_function(CTextField, setHAlign   , "args:align")
define_reflectable_class_function(CTextField, setVAlign   , "args:align")

void CTextField::setTextColor(const MColor::ptr &color) { mTextColor = color; }
void CTextField::setFontSize (float              size ) { mFontSize  = size ; }
void CTextField::setHAlign   (MHAlign            align) { mHAlign    = align; }
void CTextField::setVAlign   (MVAlign            align) { mVAlign    = align; }

define_reflectable_class_function(CTextField, text     )
define_reflectable_class_function(CTextField, entered  )
define_reflectable_class_function(CTextField, textColor)
define_reflectable_class_function(CTextField, fontSize )
define_reflectable_class_function(CTextField, hAlign   )
define_reflectable_class_function(CTextField, vAlign   )

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
    mCursorBeginTick = GetNowTick();
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
        float duration = GetNowTick() - mCursorBeginTick;
        int   gapCount = (int)(duration * 1000) / (int)(CursorInterval * 1000);

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
