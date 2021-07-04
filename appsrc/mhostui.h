#pragma once

#include "mtypes.h"

const float _MWindowDrawInterval = 0.1f;

typedef int MKey;

const MKey MKey_Back  = 0x08;
const MKey MKey_Enter = 0x0D;
const MKey MKey_Space = 0x20;
const MKey MKey_Left  = 0x25;
const MKey MKey_Up    = 0x26;
const MKey MKey_Right = 0x27;
const MKey MKey_Down  = 0x28;
const MKey MKey_A     = 'A' ;
const MKey MKey_W     = 'W' ;
const MKey MKey_D     = 'D' ;
const MKey MKey_S     = 'S' ;

typedef int MAligns;

const MAligns MAlign_Left    =  1;
const MAligns MAlign_HCenter =  2;
const MAligns MAlign_Right   =  4;
const MAligns MAlign_Top     =  8;
const MAligns MAlign_VCenter = 16;
const MAligns MAlign_Bottom  = 32;

typedef int MColor;

union MColorPattern {
    struct {
        uint8_t alpha;
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
    };
    MColor rgba;
};

//this type is used to mark the graphics size of the host ui.
typedef float _WPIXEL;

extern "C" void _MWindowSetPixelDensity(float density);

extern "C" void _MWindowOnLoad();
extern "C" void _MWindowOnShow();
extern "C" void _MWindowOnHide();

extern "C" void _MWindowOnResize(_WPIXEL width, _WPIXEL height);
extern "C" void _MWindowOnDraw  ();

extern "C" void _MWindowOnTouchBegin(_WPIXEL x, _WPIXEL y);
extern "C" void _MWindowOnTouchMove (_WPIXEL x, _WPIXEL y);
extern "C" void _MWindowOnTouchEnd  (_WPIXEL x, _WPIXEL y);
extern "C" void _MWindowOnTextBox   (MString *string, bool enter);
extern "C" void _MWindowOnKeyDown   (MKey key);

extern "C" int     _MWindowTriangleCount  ();
extern "C" _WPIXEL _MWindowTriangleVertexX(int index, int vertexIndex);
extern "C" _WPIXEL _MWindowTriangleVertexY(int index, int vertexIndex);
extern "C" MColor  _MWindowTriangleColor  (int index);

extern "C" int      _MWindowLabelCount   ();
extern "C" MString *_MWindowLabelString  (int index);
extern "C" MColor   _MWindowLabelColor   (int index);
extern "C" _WPIXEL  _MWindowLabelFontSize(int index);
extern "C" MAligns  _MWindowLabelAligns  (int index);
extern "C" _WPIXEL  _MWindowLabelX       (int index);
extern "C" _WPIXEL  _MWindowLabelY       (int index);
extern "C" _WPIXEL  _MWindowLabelWidth   (int index);
extern "C" _WPIXEL  _MWindowLabelHeight  (int index);

extern "C" bool _MWindowTextBoxEnabled();

typedef int MWindowEvent;

const MWindowEvent MWindowEvent_Load       = 'L';
const MWindowEvent MWindowEvent_Show       = 'S';
const MWindowEvent MWindowEvent_Hide       = 'H';
const MWindowEvent MWindowEvent_Resize     = 'R';
const MWindowEvent MWindowEvent_Draw       = 'D';
const MWindowEvent MWindowEvent_TouchBegin = 'B';
const MWindowEvent MWindowEvent_TouchMove  = 'M';
const MWindowEvent MWindowEvent_TouchEnd   = 'E';
const MWindowEvent MWindowEvent_TextBox    = 'T';
const MWindowEvent MWindowEvent_KeyDown    = 'K';

void MWindowAddListener(MLambda *listener);

MWindowEvent MWindowCurrentEvent();

float MWindowWidth ();
float MWindowHeight();

bool MWindowLoaded();
bool MWindowShown ();

float MWindowTouchX();
float MWindowTouchY();
MKey  MWindowActiveKey();

void MWindowSelectString  (MString *string);
void MWindowSelectColor   (MColor   color );
void MWindowSelectFontSize(float    size  );
void MWindowSelectAligns  (MAligns  aligns);

void MWindowSelectPoint0(float x, float y);
void MWindowSelectPoint1(float x, float y);
void MWindowSelectPoint2(float x, float y);

void MWindowDrawTriangle();
void MWindowDrawLabel();

void     MWindowEnableTextBox(bool enabled);
MString *MWindowTextBoxString();
bool     MWindowTextBoxEnter ();

MEXPORT(MWindowAddListener   )
MEXPORT(MWindowCurrentEvent  )
MEXPORT(MWindowWidth         )
MEXPORT(MWindowHeight        )
MEXPORT(MWindowLoaded        )
MEXPORT(MWindowShown         )
MEXPORT(MWindowTouchX        )
MEXPORT(MWindowTouchY        )
MEXPORT(MWindowActiveKey     )
MEXPORT(MWindowSelectString  )
MEXPORT(MWindowSelectColor   )
MEXPORT(MWindowSelectFontSize)
MEXPORT(MWindowSelectAligns  )
MEXPORT(MWindowSelectPoint0  )
MEXPORT(MWindowSelectPoint1  )
MEXPORT(MWindowSelectPoint2  )
MEXPORT(MWindowDrawTriangle  )
MEXPORT(MWindowDrawLabel     )
MEXPORT(MWindowEnableTextBox )
MEXPORT(MWindowTextBoxString )
MEXPORT(MWindowTextBoxEnter  )
