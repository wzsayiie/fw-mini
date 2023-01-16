#include "mevent.h"
#include "dlazy.h"
#include "mgraphics.h"
#include "rdefine.h"

//event:

define_reflectable_enum_const(MEvent, Touch     )
define_reflectable_enum_const(MEvent, MouseMove )
define_reflectable_enum_const(MEvent, MouseWheel)
define_reflectable_enum_const(MEvent, KbKey     )
define_reflectable_enum_const(MEvent, Writing   )

//touch event:

define_reflectable_enum_const(MTouchSource, Finger )
define_reflectable_enum_const(MTouchSource, LButton)

define_reflectable_enum_const(MTouchStep, Begin)
define_reflectable_enum_const(MTouchStep, Move )
define_reflectable_enum_const(MTouchStep, End  )

define_reflectable_class_function(MTouch, makeBeginPixel, "args:pixelX,pixelY,source")
MTouch::ptr MTouch::makeBeginPixel(float pixelX, float pixelY, MTouchSource source) {
    auto evt = MTouch::create();
    
    evt->mSource = source;
    evt->mStep   = MTouchStep::Begin;
    evt->mX      = m_pt_from_px pixelX;
    evt->mY      = m_pt_from_px pixelY;
    
    return evt;
}

define_reflectable_class_function(MTouch, makeMovePixel, "args:pixelX,pixelY,source")
MTouch::ptr MTouch::makeMovePixel(float pixelX, float pixelY, MTouchSource source) {
    auto evt = MTouch::create();
    
    evt->mSource = source;
    evt->mStep   = MTouchStep::Move;
    evt->mX      = m_pt_from_px pixelX;
    evt->mY      = m_pt_from_px pixelY;
    
    return evt;
}

define_reflectable_class_function(MTouch, makeEndPixel, "args:pixelX,pixelY,source")
MTouch::ptr MTouch::makeEndPixel(float pixelX, float pixelY, MTouchSource source) {
    auto evt = MTouch::create();
    
    evt->mSource = source;
    evt->mStep   = MTouchStep::End;
    evt->mX      = m_pt_from_px pixelX;
    evt->mY      = m_pt_from_px pixelY;
    
    return evt;
}

define_reflectable_class_function(MTouch, source, "getter")
define_reflectable_class_function(MTouch, step  , "getter")
define_reflectable_class_function(MTouch, x     , "getter")
define_reflectable_class_function(MTouch, y     , "getter")

MTouchSource MTouch::source() { return mSource; }
MTouchStep   MTouch::step  () { return mStep  ; }
float        MTouch::x     () { return mX     ; }
float        MTouch::y     () { return mY     ; }

//mouse move event:

define_reflectable_class_function(MMouseMove, makePixel, "args:pixelX,pixelY")
MMouseMove::ptr MMouseMove::makePixel(float pixelX, float pixelY) {
    auto evt = MMouseMove::create();
    
    evt->mX = m_pt_from_px pixelX;
    evt->mY = m_pt_from_px pixelY;
    
    return evt;
}

define_reflectable_class_function(MMouseMove, x, "getter")
define_reflectable_class_function(MMouseMove, y, "getter")

float MMouseMove::x() { return mX; }
float MMouseMove::y() { return mY; }

//mouse wheel event:

define_reflectable_class_function(MMouseWheel, makePixel, "args:delta,pixelX,pixelY")
MMouseWheel::ptr MMouseWheel::makePixel(float delta, float pixelX, float pixelY) {
    auto evt = MMouseWheel::create();
    
    evt->mX     = m_pt_from_px pixelX;
    evt->mY     = m_pt_from_px pixelY;
    evt->mDelta = delta;
    
    return evt;
}

define_reflectable_class_function(MMouseWheel, x    , "getter")
define_reflectable_class_function(MMouseWheel, y    , "getter")
define_reflectable_class_function(MMouseWheel, delta, "getter")

float MMouseWheel::x    () { return mX    ; }
float MMouseWheel::y    () { return mY    ; }
float MMouseWheel::delta() { return mDelta; }

//keyboard key event:

define_reflectable_const(MKbModifier_Alt  )
define_reflectable_const(MKbModifier_Caps )
define_reflectable_const(MKbModifier_Cmd  )
define_reflectable_const(MKbModifier_Ctrl )
define_reflectable_const(MKbModifier_Shift)

define_reflectable_enum_const(MKbKeyCode, Null )
define_reflectable_enum_const(MKbKeyCode, Back )
define_reflectable_enum_const(MKbKeyCode, Tab  )
define_reflectable_enum_const(MKbKeyCode, Enter)
define_reflectable_enum_const(MKbKeyCode, Space)
define_reflectable_enum_const(MKbKeyCode, Left )
define_reflectable_enum_const(MKbKeyCode, Up   )
define_reflectable_enum_const(MKbKeyCode, Right)
define_reflectable_enum_const(MKbKeyCode, Down )
define_reflectable_enum_const(MKbKeyCode, A    )
define_reflectable_enum_const(MKbKeyCode, D    )
define_reflectable_enum_const(MKbKeyCode, S    )
define_reflectable_enum_const(MKbKeyCode, W    )

define_reflectable_class_function(MKbKey, make, "args:code,modifiers")
MKbKey::ptr MKbKey::make(MKbKeyCode code, MKbModifiers modifiers) {
    auto evt = MKbKey::create();

    evt->mCode      = code     ;
    evt->mModifiers = modifiers;

    return evt;
}

define_reflectable_class_function(MKbKey, code, "getter")
MKbKeyCode MKbKey::code() {
    return mCode;
}

define_reflectable_class_function(MKbKey, modifiers, "getter")
MKbModifiers MKbKey::modifiers() {
    return mModifiers;
}

define_reflectable_class_function(MKbKey, printable, "getter")
int MKbKey::printable() {
    if (mCode == MKbKeyCode::Tab  ) { return '\t'; }
    if (mCode == MKbKeyCode::Enter) { return '\n'; }
    if (mCode == MKbKeyCode::Space) { return ' ' ; }

    if ('A' <= (int)mCode && (int)mCode <= 'Z') {
        if ((!(mModifiers & MKbModifier_Caps) && !(mModifiers & MKbModifier_Shift)) ||
            ( (mModifiers & MKbModifier_Caps) &&  (mModifiers & MKbModifier_Shift)) )
        {
            //lowercase.
            return (int)mCode - 'A' + 'a';
        }
        else
        {
            //uppercase.
            return (int)mCode;
        }
    }

    return '?';
}

//text writing event:

define_reflectable_class_function(MWriting, make, "args:text")
MWriting::ptr MWriting::make(const std::string &text) {
    auto evt = MWriting::create();
    evt->mText = text;
    return evt;
}

define_reflectable_class_function(MWriting, text, "getter")
std::string MWriting::text() {
    return mText;
}

//current events:

static dash::lazy<MTouch::ptr     > sTouch     ;
static dash::lazy<MMouseMove::ptr > sMouseMove ;
static dash::lazy<MMouseWheel::ptr> sMouseWheel;
static dash::lazy<MKbKey::ptr     > sKbKey     ;
static dash::lazy<MWriting::ptr   > sWriting   ;

define_reflectable_function(MResetCurrentTouch     , "args:evt")
define_reflectable_function(MResetCurrentMouseMove , "args:evt")
define_reflectable_function(MResetCurrentMouseWheel, "args:evt")
define_reflectable_function(MResetCurrentKbKey     , "args:evt")
define_reflectable_function(MResetCurrentWriting   , "args:evt")

void MResetCurrentTouch     (const MTouch::ptr      &evt) { sTouch      = evt; }
void MResetCurrentMouseMove (const MMouseMove::ptr  &evt) { sMouseMove  = evt; }
void MResetCurrentMouseWheel(const MMouseWheel::ptr &evt) { sMouseWheel = evt; }
void MResetCurrentKbKey     (const MKbKey::ptr      &evt) { sKbKey      = evt; }
void MResetCurrentWriting   (const MWriting::ptr    &evt) { sWriting    = evt; }

define_reflectable_function(MCurrentTouch     )
define_reflectable_function(MCurrentMouseMove )
define_reflectable_function(MCurrentMouseWheel)
define_reflectable_function(MCurrentKbKey     )
define_reflectable_function(MCurrentWriting   )

MTouch::ptr      MCurrentTouch     () { return sTouch     ; }
MMouseMove::ptr  MCurrentMouseMove () { return sMouseMove ; }
MMouseWheel::ptr MCurrentMouseWheel() { return sMouseWheel; }
MKbKey::ptr      MCurrentKbKey     () { return sKbKey     ; }
MWriting::ptr    MCurrentWriting   () { return sWriting   ; }
