#pragma once

#include "mexport.h"
#include "mtypes.h"

static const char     *const _MAppWindowU8Name  =  "Mini";
static const char16_t *const _MAppWindowU16Name = u"Mini";

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

typedef int MHAlign;

const MHAlign MHAlign_Left   = 1;
const MHAlign MHAlign_Center = 2;
const MHAlign MHAlign_Right  = 3;

typedef int MVAlign;

const MVAlign MVAlign_Top    = 1;
const MVAlign MVAlign_Center = 2;
const MVAlign MVAlign_Bottom = 3;

typedef int MColor;

union MColorPattern {
    struct {
        uint8_t alpha;
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
    };
    MColor rgba = 0;
};

//this type is used to mark the graphics size of the host ui.
typedef float _MPixel;

extern "C" void _MWindowSetPixelDensity(float density);

extern "C" void _MWindowOnLoad();
extern "C" void _MWindowOnShow();
extern "C" void _MWindowOnHide();

extern "C" void _MWindowOnResize(_MPixel width, _MPixel height);
extern "C" void _MWindowOnDraw  ();

extern "C" void _MWindowOnTouchBegin(_MPixel x, _MPixel y);
extern "C" void _MWindowOnTouchMove (_MPixel x, _MPixel y);
extern "C" void _MWindowOnTouchEnd  (_MPixel x, _MPixel y);
extern "C" void _MWindowOnTextBox   (MString *string, bool enter);
extern "C" void _MWindowOnKeyDown   (MKey key);

extern "C" int     _MWindowTriangleCount  ();
extern "C" _MPixel _MWindowTriangleVertexX(int index, int vertexIndex);
extern "C" _MPixel _MWindowTriangleVertexY(int index, int vertexIndex);
extern "C" MColor  _MWindowTriangleColor  (int index);

extern "C" int     _MWindowImageCount ();
extern "C" MImage *_MWindowImageObject(int index);
extern "C" _MPixel _MWindowImageX     (int index);
extern "C" _MPixel _MWindowImageY     (int index);
extern "C" _MPixel _MWindowImageWidth (int index);
extern "C" _MPixel _MWindowImageHeight(int index);

extern "C" int      _MWindowLabelCount   ();
extern "C" MString *_MWindowLabelString  (int index);
extern "C" MColor   _MWindowLabelColor   (int index);
extern "C" _MPixel  _MWindowLabelFontSize(int index);
extern "C" MHAlign  _MWindowLabelHAlign  (int index);
extern "C" MVAlign  _MWindowLabelVAlign  (int index);
extern "C" _MPixel  _MWindowLabelX       (int index);
extern "C" _MPixel  _MWindowLabelY       (int index);
extern "C" _MPixel  _MWindowLabelWidth   (int index);
extern "C" _MPixel  _MWindowLabelHeight  (int index);

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
void MWindowSelectImage   (MImage  *image );
void MWindowSelectColor   (MColor   color );
void MWindowSelectFontSize(float    size  );
void MWindowSelectHAlign  (MHAlign  align );
void MWindowSelectVAlign  (MVAlign  align );

void MWindowSelectPoint0(float x, float y);
void MWindowSelectPoint1(float x, float y);
void MWindowSelectPoint2(float x, float y);

void MWindowDrawTriangle();
void MWindowDrawImage();
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
MEXPORT(MWindowSelectHAlign  )
MEXPORT(MWindowSelectVAlign  )
MEXPORT(MWindowSelectPoint0  )
MEXPORT(MWindowSelectPoint1  )
MEXPORT(MWindowSelectPoint2  )
MEXPORT(MWindowDrawTriangle  )
MEXPORT(MWindowDrawLabel     )
MEXPORT(MWindowEnableTextBox )
MEXPORT(MWindowTextBoxString )
MEXPORT(MWindowTextBoxEnter  )
