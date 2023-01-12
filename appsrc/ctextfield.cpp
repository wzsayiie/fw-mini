#include "ctextfield.h"

//text field delegation:

define_reflectable_class_function(CTextFieldDelegation, setEditingBeginTarget, "setter;args:target")
define_reflectable_class_function(CTextFieldDelegation, setTextChangeTarget  , "setter;args:target")
define_reflectable_class_function(CTextFieldDelegation, setEditingEndTarget  , "setter;args:target")

void CTextFieldDelegation::setEditingBeginTarget(const MFunction<void ()>::ptr &target) { mEditingBeginTarget = target; }
void CTextFieldDelegation::setTextChangeTarget  (const MFunction<void ()>::ptr &target) { mTextChangeTarget   = target; }
void CTextFieldDelegation::setEditingEndTarget  (const MFunction<void ()>::ptr &target) { mEditingEndTarget   = target; }

define_reflectable_class_function(CTextFieldDelegation, editingBeginTarget, "getter")
define_reflectable_class_function(CTextFieldDelegation, textChangeTarget  , "getter")
define_reflectable_class_function(CTextFieldDelegation, editingEndTarget  , "getter")

MFunction<void ()>::ptr CTextFieldDelegation::editingBeginTarget() { return mEditingBeginTarget; }
MFunction<void ()>::ptr CTextFieldDelegation::textChangeTarget  () { return mTextChangeTarget  ; }
MFunction<void ()>::ptr CTextFieldDelegation::editingEndTarget  () { return mEditingEndTarget  ; }

define_reflectable_class_function(CTextFieldDelegation, editingBegin)
define_reflectable_class_function(CTextFieldDelegation, textChange  )
define_reflectable_class_function(CTextFieldDelegation, editingEnd  )

void CTextFieldDelegation::editingBegin() { if (mEditingBeginTarget) { mEditingBeginTarget->call(); } }
void CTextFieldDelegation::textChange  () { if (mTextChangeTarget  ) { mTextChangeTarget  ->call(); } }
void CTextFieldDelegation::editingEnd  () { if (mEditingEndTarget  ) { mEditingEndTarget  ->call(); } }

void CTextFieldDelegation::on_dispose() {
    super::dispose();

    mEditingBeginTarget = nullptr;
    mTextChangeTarget   = nullptr;
    mEditingEndTarget   = nullptr;
}

//text field:

define_reflectable_class_function(CTextField, setDelegation, "setter;args:delegation")
void CTextField::setDelegation(const CTextFieldDelegation::ptr &delegation) {
    mDelegation = delegation;
}

define_reflectable_class_function(CTextField, delegation, "getter")
CTextFieldDelegation::ptr CTextField::delegation() {
    if (!mDelegation) {
        mDelegation = CTextFieldDelegation::create();
    }
    return mDelegation;
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
define_reflectable_class_function(CTextField, entered  , "getter")

std::string CTextField::text     () { return mText    ; }
MColor::ptr CTextField::textColor() { return mTextColor ? mTextColor : MColor::clearColor(); }
float       CTextField::fontSize () { return mFontSize; }
MHAlign     CTextField::hAlign   () { return mHAlign  ; }
MVAlign     CTextField::vAlign   () { return mVAlign  ; }
bool        CTextField::entered  () { return mEntered ; }

void CTextField::onBecomeFocusResponder() {
    increaseEditingSender();

    MWindow::mainWindow()->setWritingEnabled(true, mText);
    mCursorBeginTick = MScheduler::instance()->secondsTick();
}

void CTextField::onResignFocusResponder() {
    reduceEditingSender();

    MWindow::mainWindow()->setWritingEnabled(false, "");
    mCursorBeginTick = 0;
}

void CTextField::onWriting(const std::string &text) {
    mText = text;
    sendEditing();
}

void CTextField::onControlKbKey(MKbKeyCode code) {
    if (code == MKbKeyCode::Enter) {
        mEntered = true;
        transferFocusToAny();
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
        double now      = MScheduler::instance()->secondsTick();
        double duration = now - mCursorBeginTick;
        auto   count    = (int64_t)(duration * 1000) / (int64_t)(interval * 1000);

        if (count % 2) {
            MContextSelectRGBA(mTextColor->rgba() & 0xFFffFF40);
        } else {
            MContextSelectRGBA(mTextColor->rgba());
        }
        
        MContextDrawRect(0,              0, width, thick ); //top.
        MContextDrawRect(0, height - thick, width, thick ); //bottom.
        MContextDrawRect(0,              0, thick, height); //left.
        MContextDrawRect(width - thick,  0, thick, height); //right.
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

    if (mEditingSenders == 1) {
        //NOTE: reset "entered" flag.
        mEntered = false;
        delegation()->editingBegin();
    }
}

void CTextField::sendEditing() {
    if (mEditingSenders <= 0) {
        return;
    }
    if (mLastText == mText) {
        return;
    }

    mLastText = mText;
    delegation()->textChange();
}

void CTextField::reduceEditingSender() {
    mEditingSenders -= 1;

    if (mEditingSenders == 0) {
        delegation()->editingEnd();
    }
}

void CTextField::on_dispose() {
    super::dispose();

    if (mDelegation) {
        mDelegation->dispose();
    }
}
