#include "clabel.h"

define_reflectable_class_function(CLabel, setText     , "setter;args:text" )
define_reflectable_class_function(CLabel, setTextColor, "setter;args:color")
define_reflectable_class_function(CLabel, setFontSize , "setter;args:size" )
define_reflectable_class_function(CLabel, setHAlign   , "setter;args:align")
define_reflectable_class_function(CLabel, setVAlign   , "setter;args:align")

void CLabel::setText     (const std::string &text ) { mText      = text ; }
void CLabel::setTextColor(const MColor::ptr &color) { mTextColor = color; }
void CLabel::setFontSize (float              size ) { mFontSize  = size ; }
void CLabel::setHAlign   (MHAlign            align) { mHAlign    = align; }
void CLabel::setVAlign   (MVAlign            align) { mVAlign    = align; }

define_reflectable_class_function(CLabel, text     , "getter")
define_reflectable_class_function(CLabel, textColor, "getter")
define_reflectable_class_function(CLabel, fontSize , "getter")
define_reflectable_class_function(CLabel, hAlign   , "getter")
define_reflectable_class_function(CLabel, vAlign   , "getter")

std::string CLabel::text     () { return mText    ; }
MColor::ptr CLabel::textColor() { return mTextColor ? mTextColor : MColor::clearColor(); }
float       CLabel::fontSize () { return mFontSize; }
MHAlign     CLabel::hAlign   () { return mHAlign  ; }
MVAlign     CLabel::vAlign   () { return mVAlign  ; }

void CLabel::onDraw(float width, float height) {
    if (mText.empty()) {
        return;
    }
    if (!mTextColor || mTextColor->none()) {
        return;
    }
    if (mFontSize <= 0) {
        return;
    }

    MContextSelectText    (mText    );
    MContextSelectRGBA    (mTextColor->rgba());
    MContextSelectFontSize(mFontSize);
    MContextSelectHAlign  (mHAlign  );
    MContextSelectVAlign  (mVAlign  );

    MContextDrawText(0, 0, width, height);
}
