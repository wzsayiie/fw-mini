#include "mevent.h"

//touch event:

define_reflectable_class_function(MTouch, makeBegin, "args:x,y,source")
MTouch::ptr MTouch::makeBegin(float x, float y, MTouchSource source) {
    auto event = MTouch::create();
    
    event->mSource = source;
    event->mStep   = MTouchStep::Begin;
    event->mX      = x;
    event->mY      = y;
    
    return event;
}

define_reflectable_class_function(MTouch, makeMove, "args:x,y,source")
MTouch::ptr MTouch::makeMove(float x, float y, MTouchSource source) {
    auto event = MTouch::create();
    
    event->mSource = source;
    event->mStep   = MTouchStep::Move;
    event->mX      = x;
    event->mY      = y;
    
    return event;
}

define_reflectable_class_function(MTouch, makeEnd, "args:x,y,source")
MTouch::ptr MTouch::makeEnd(float x, float y, MTouchSource source) {
    auto event = MTouch::create();
    
    event->mSource = source;
    event->mStep   = MTouchStep::End;
    event->mX      = x;
    event->mY      = y;
    
    return event;
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

define_reflectable_class_function(MMouseMove, make, "args:x,y")
MMouseMove::ptr MMouseMove::make(float x, float y) {
    auto event = MMouseMove::create();
    
    event->mX = x;
    event->mY = y;
    
    return event;
}

define_reflectable_class_function(MMouseMove, x, "getter")
define_reflectable_class_function(MMouseMove, y, "getter")

float MMouseMove::x() { return mX; }
float MMouseMove::y() { return mY; }

//mouse wheel event:

define_reflectable_class_function(MMouseWheel, make, "args:delta,x,y")
MMouseWheel::ptr MMouseWheel::make(float delta, float x, float y) {
    auto event = MMouseWheel::create();
    
    event->mDelta = delta;
    event->mX     = x    ;
    event->mY     = y    ;
    
    return event;
}

define_reflectable_class_function(MMouseWheel, delta, "getter")
define_reflectable_class_function(MMouseWheel, x    , "getter")
define_reflectable_class_function(MMouseWheel, y    , "getter")

float MMouseWheel::delta() { return mDelta; }
float MMouseWheel::x    () { return mX    ; }
float MMouseWheel::y    () { return mY    ; }

//keyboard key event:

//text writing event:

define_reflectable_class_function(MWriting, make, "args:text")
MWriting::ptr MWriting::make(const std::string &text) {
    auto event = MWriting::create();
    event->mText = text;
    return event;
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

define_reflectable_function(MResetCurrentTouch     , "args:event")
define_reflectable_function(MResetCurrentMouseMove , "args:event")
define_reflectable_function(MResetCurrentMouseWheel, "args:event")
define_reflectable_function(MResetCurrentKbKey     , "args:event")
define_reflectable_function(MResetCurrentWriting   , "args:event")

void MResetCurrentTouch     (const MTouch::ptr      &event) { sTouch      = event; }
void MResetCurrentMouseMove (const MMouseMove::ptr  &event) { sMouseMove  = event; }
void MResetCurrentMouseWheel(const MMouseWheel::ptr &event) { sMouseWheel = event; }
void MResetCurrentKbKey     (const MKbKey::ptr      &event) { sKbKey      = event; }
void MResetCurrentWriting   (const MWriting::ptr    &event) { sWriting    = event; }

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
