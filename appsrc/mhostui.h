#pragma once

#include "mtypes.h"

static const char     *const _MWindowTitleU8Name  =  "Mini";
static const char16_t *const _MWindowTitleU16Name = u"Mini";

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

const MHAlign MHAlign_Left   = 'L';
const MHAlign MHAlign_Center = 'C';
const MHAlign MHAlign_Right  = 'R';

typedef int MVAlign;

const MVAlign MVAlign_Top    = 'T';
const MVAlign MVAlign_Center = 'C';
const MVAlign MVAlign_Bottom = 'B';

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

MFUNC_HOST void _MWindowSetPixelDensity(float density);

MFUNC_HOST void _MWindowOnLoad();
MFUNC_HOST void _MWindowOnShow();
MFUNC_HOST void _MWindowOnHide();

MFUNC_HOST void _MWindowOnResize(_MPixel width, _MPixel height);
MFUNC_HOST void _MWindowOnDraw  ();

MFUNC_HOST void _MWindowOnTouchBegin(_MPixel x, _MPixel y);
MFUNC_HOST void _MWindowOnTouchMove (_MPixel x, _MPixel y);
MFUNC_HOST void _MWindowOnTouchEnd  (_MPixel x, _MPixel y);
MFUNC_HOST void _MWindowOnTextBox   (MString *string, bool enter);
MFUNC_HOST void _MWindowOnKeyDown   (MKey key);

MFUNC_HOST int     _MWindowTriangleCount  ();
MFUNC_HOST _MPixel _MWindowTriangleVertexX(int index, int vertexIndex);
MFUNC_HOST _MPixel _MWindowTriangleVertexY(int index, int vertexIndex);
MFUNC_HOST MColor  _MWindowTriangleColor  (int index);

MFUNC_HOST int     _MWindowImageCount ();
MFUNC_HOST MImage *_MWindowImageObject(int index);
MFUNC_HOST _MPixel _MWindowImageX     (int index);
MFUNC_HOST _MPixel _MWindowImageY     (int index);
MFUNC_HOST _MPixel _MWindowImageWidth (int index);
MFUNC_HOST _MPixel _MWindowImageHeight(int index);

MFUNC_HOST int      _MWindowLabelCount   ();
MFUNC_HOST MString *_MWindowLabelString  (int index);
MFUNC_HOST MColor   _MWindowLabelColor   (int index);
MFUNC_HOST _MPixel  _MWindowLabelFontSize(int index);
MFUNC_HOST MHAlign  _MWindowLabelHAlign  (int index);
MFUNC_HOST MVAlign  _MWindowLabelVAlign  (int index);
MFUNC_HOST _MPixel  _MWindowLabelX       (int index);
MFUNC_HOST _MPixel  _MWindowLabelY       (int index);
MFUNC_HOST _MPixel  _MWindowLabelWidth   (int index);
MFUNC_HOST _MPixel  _MWindowLabelHeight  (int index);

MFUNC_HOST bool _MWindowTextBoxEnabled();

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

MFUNC_EXPORT(MWindowAddListener   )
MFUNC_EXPORT(MWindowCurrentEvent  )
MFUNC_EXPORT(MWindowWidth         )
MFUNC_EXPORT(MWindowHeight        )
MFUNC_EXPORT(MWindowLoaded        )
MFUNC_EXPORT(MWindowShown         )
MFUNC_EXPORT(MWindowTouchX        )
MFUNC_EXPORT(MWindowTouchY        )
MFUNC_EXPORT(MWindowActiveKey     )
MFUNC_EXPORT(MWindowSelectString  )
MFUNC_EXPORT(MWindowSelectColor   )
MFUNC_EXPORT(MWindowSelectFontSize)
MFUNC_EXPORT(MWindowSelectHAlign  )
MFUNC_EXPORT(MWindowSelectVAlign  )
MFUNC_EXPORT(MWindowSelectPoint0  )
MFUNC_EXPORT(MWindowSelectPoint1  )
MFUNC_EXPORT(MWindowSelectPoint2  )
MFUNC_EXPORT(MWindowDrawTriangle  )
MFUNC_EXPORT(MWindowDrawLabel     )
MFUNC_EXPORT(MWindowEnableTextBox )
MFUNC_EXPORT(MWindowTextBoxString )
MFUNC_EXPORT(MWindowTextBoxEnter  )
