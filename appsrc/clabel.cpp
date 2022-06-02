#include "clabel.h"

define_reflectable_class_function(CLabel, setText     , "type:set;args:text" )
define_reflectable_class_function(CLabel, setTextColor, "type:set;args:color")
define_reflectable_class_function(CLabel, setFontSize , "type:set;args:size" )
define_reflectable_class_function(CLabel, setHAlign   , "type:set;args:align")
define_reflectable_class_function(CLabel, setVAlign   , "type:set;args:align")

void CLabel::setText     (const std::string &text ) { mText      = text ; }
void CLabel::setTextColor(const MColor::ptr &color) { mTextColor = color; }
void CLabel::setFontSize (float              size ) { mFontSize  = size ; }
void CLabel::setHAlign   (MHAlign            align) { mHAlign    = align; }
void CLabel::setVAlign   (MVAlign            align) { mVAlign    = align; }

define_reflectable_class_function(CLabel, text     , "type:get")
define_reflectable_class_function(CLabel, textColor, "type:get")
define_reflectable_class_function(CLabel, fontSize , "type:get")
define_reflectable_class_function(CLabel, hAlign   , "type:get")
define_reflectable_class_function(CLabel, vAlign   , "type:get")

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
