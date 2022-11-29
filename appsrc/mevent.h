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
    static MTouch::ptr makeBeginPixel(float pixelX, float pixelY, MTouchSource source);
    static MTouch::ptr makeMovePixel (float pixelX, float pixelY, MTouchSource source);
    static MTouch::ptr makeEndPixel  (float pixelX, float pixelY, MTouchSource source);
    
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
    static MMouseMove::ptr makePixel(float pixelX, float pixelY);
    
    float x();
    float y();
    
private:
    float mX = 0;
    float mY = 0;
};

//mouse wheel event:

m_class(MMouseWheel, MObject) {
public:
    static MMouseWheel::ptr makePixel(float pixelX, float pixelY, float delta);
    
    float x    ();
    float y    ();
    float delta();

private:
    float mX     = 0;
    float mY     = 0;
    float mDelta = 0;
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

d_exportable void MResetCurrentTouch     (const MTouch::ptr      &evt);
d_exportable void MResetCurrentMouseMove (const MMouseMove::ptr  &evt);
d_exportable void MResetCurrentMouseWheel(const MMouseWheel::ptr &evt);
d_exportable void MResetCurrentKbKey     (const MKbKey::ptr      &evt);
d_exportable void MResetCurrentWriting   (const MWriting::ptr    &evt);

d_exportable MTouch::ptr      MCurrentTouch     ();
d_exportable MMouseMove::ptr  MCurrentMouseMove ();
d_exportable MMouseWheel::ptr MCurrentMouseWheel();
d_exportable MKbKey::ptr      MCurrentKbKey     ();
d_exportable MWriting::ptr    MCurrentWriting   ();
