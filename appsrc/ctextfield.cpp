#include "ctextfield.h"
#include "mcontext.h"
#include "mwindow.h"
#include "rdefine.h"

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
    base::dispose();

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

define_reflectable_class_function(CTextField, setText     , "setter;args:text"  )
define_reflectable_class_function(CTextField, setPrompt   , "setter;args:prompt")
define_reflectable_class_function(CTextField, setTextColor, "setter;args:color" )
define_reflectable_class_function(CTextField, setFontSize , "setter;args:size"  )
define_reflectable_class_function(CTextField, setHAlign   , "setter;args:align" )
define_reflectable_class_function(CTextField, setVAlign   , "setter;args:align" )

void CTextField::setText(const std::string &text) {
    mText = text;

    increaseEditingSender();
    sendEditing();
    reduceEditingSender();
}

void CTextField::setPrompt   (const std::string &prompt) { mPrompt    = prompt; }
void CTextField::setTextColor(const MColor::ptr &color ) { mTextColor = color ; }
void CTextField::setFontSize (float              size  ) { mFontSize  = size  ; }
void CTextField::setHAlign   (MHAlign            align ) { mHAlign    = align ; }
void CTextField::setVAlign   (MVAlign            align ) { mVAlign    = align ; }

define_reflectable_class_function(CTextField, text     , "getter")
define_reflectable_class_function(CTextField, prompt   , "getter")
define_reflectable_class_function(CTextField, textColor, "getter")
define_reflectable_class_function(CTextField, fontSize , "getter")
define_reflectable_class_function(CTextField, hAlign   , "getter")
define_reflectable_class_function(CTextField, vAlign   , "getter")
define_reflectable_class_function(CTextField, entered  , "getter")

std::string CTextField::text     () { return mText    ; }
std::string CTextField::prompt   () { return mPrompt  ; }
MColor::ptr CTextField::textColor() { return mTextColor ? mTextColor : MColor::clearColor(); }
float       CTextField::fontSize () { return mFontSize; }
MHAlign     CTextField::hAlign   () { return mHAlign  ; }
MVAlign     CTextField::vAlign   () { return mVAlign  ; }
bool        CTextField::entered  () { return mEntered ; }

void CTextField::onBecomeFocusResponder() {
    increaseEditingSender();
    MWindow::mainWindow()->setWritingEnabled(true, mText);
}

void CTextField::onResignFocusResponder() {
    reduceEditingSender();
    MWindow::mainWindow()->setWritingEnabled(false, "");
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

void CTextField::onDraw(float width, float height) {
    if (!mTextColor || mTextColor->alphaCom() == 0) {
        return;
    }
    if (mFontSize <= 0) {
        return;
    }
    
    auto text = mText;
    int  rgba = mTextColor->rgba();
    
    if (text.empty()) {
        text = mPrompt;
        rgba = rgba & 0xFFffFF40;
    }
    
    MContextSelectText    (text     );
    MContextSelectRGBA    (rgba     );
    MContextSelectFontSize(mFontSize);
    MContextSelectHAlign  (mHAlign  );
    MContextSelectVAlign  (mVAlign  );
    
    MContextDrawText(0, 0, width, height);
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
    base::dispose();

    if (mDelegation) {
        mDelegation->dispose();
    }
}
