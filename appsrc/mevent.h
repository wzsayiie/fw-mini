#pragma once

#include "mobject.h"

//event:

m_enum(MEvent) {
    Touch      = 1,
    MouseMove  = 2,
    MouseWheel = 3,
    KbKey      = 4,
    Writing    = 5,
};

//touch event:

m_enum(MTouchSource) {
    Finger  = 1,
    LButton = 1,
};

m_enum(MTouchStep) {
    Begin = 1,
    Move  = 2,
    End   = 3,
};

m_class(MTouch, MObject) {
public:
    static MTouch::ptr makeBegin(float x, float y, MTouchSource source);
    static MTouch::ptr makeMove (float x, float y, MTouchSource source);
    static MTouch::ptr makeEnd  (float x, float y, MTouchSource source);
    
    MTouchSource source();
    MTouchStep   step  ();
    float        x     ();
    float        y     ();
    
private:
    MTouchSource mSource = MTouchSource::Finger;
    MTouchStep   mStep   = MTouchStep  ::Begin ;
    float        mX      = 0;
    float        mY      = 0;
};

//mouse move event:

m_class(MMouseMove, MObject) {
public:
    static MMouseMove::ptr make(float x, float y);
    
    float x();
    float y();
    
private:
    float mX = 0;
    float mY = 0;
};

//mouse wheel event:

m_class(MMouseWheel, MObject) {
public:
    static MMouseWheel::ptr make(float delta, float x, float y);
    
    float delta();
    float x    ();
    float y    ();

private:
    float mDelta = 0;
    float mX     = 0;
    float mY     = 0;
};

//keyboard key event:

typedef int MKbModifiers;

const int MKbModifier_Alt   = 1     ;
const int MKbModifier_Caps  = 1 << 1;
const int MKbModifier_Cmd   = 1 << 2;
const int MKbModifier_Ctrl  = 1 << 3;
const int MKbModifier_Shift = 1 << 4;

m_enum(MKbKeyCode) {
    Null  = 0,

    Back  = 0x08,
    Tab   = 0x09,
    Enter = 0x0D,
    Space = 0x20,

    Left  = 0x25,
    Up    = 0x26,
    Right = 0x27,
    Down  = 0x28,

    A     = 'A' ,
    D     = 'D' ,
    S     = 'S' ,
    W     = 'W' ,
};

m_class(MKbKey, MObject) {
public:
    static MKbKey::ptr make(MKbKeyCode code, MKbModifiers modifiers);
    
    MKbKeyCode   code     ();
    MKbModifiers modifiers();
    int          printable();
    
private:
    MKbKeyCode   mCode      = MKbKeyCode::Null;
    MKbModifiers mModifiers = 0;
};

//text writing event:

m_class(MWriting, MObject) {
public:
    static MWriting::ptr make(const std::string &text);
    
    std::string text();
    
private:
    std::string mText;
};

//current events:

void MResetCurrentTouch     (const MTouch::ptr      &event);
void MResetCurrentMouseMove (const MMouseMove::ptr  &event);
void MResetCurrentMouseWheel(const MMouseWheel::ptr &event);
void MResetCurrentKbKey     (const MKbKey::ptr      &event);
void MResetCurrentWriting   (const MWriting::ptr    &event);

MTouch::ptr      MCurrentTouch     ();
MMouseMove::ptr  MCurrentMouseMove ();
MMouseWheel::ptr MCurrentMouseWheel();
MKbKey::ptr      MCurrentKbKey     ();
MWriting::ptr    MCurrentWriting   ();
