#pragma once

#include "mtypes.h"

//------------------------------------------------------------------------------
//configuration:

static const char     *const _MWindowTitleU8Name  =  "Mini";
static const char16_t *const _MWindowTitleU16Name = u"Mini";

const float _MWindowDrawInterval = 0.1f;

//------------------------------------------------------------------------------
//graph:

typedef int _MGraph;

enum {
    _MGraph_Triangle = 'T',
    _MGraph_Label    = 'L',
    _MGraph_Image    = 'I',
};

//------------------------------------------------------------------------------
//alignment:

typedef int MHAlign;

enum {
    MHAlign_Left   = 'L',
    MHAlign_Center = 'C',
    MHAlign_Right  = 'R',
};

typedef int MVAlign;

enum {
    MVAlign_Top    = 'T',
    MVAlign_Center = 'C',
    MVAlign_Bottom = 'B',
};

//------------------------------------------------------------------------------
//color:

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

//------------------------------------------------------------------------------
//key:

typedef int MKey;

enum {
    MKey_Back  = 0x08,
    MKey_Enter = 0x0D,
    MKey_Space = 0x20,
    MKey_Left  = 0x25,
    MKey_Up    = 0x26,
    MKey_Right = 0x27,
    MKey_Down  = 0x28,
    MKey_A     = 'A' ,
    MKey_W     = 'W' ,
    MKey_D     = 'D' ,
    MKey_S     = 'S' ,
};

//------------------------------------------------------------------------------
//window event:

typedef int MWindowEvent;

enum {
    MWindowEvent_Load       = 'L',
    MWindowEvent_Show       = 'S',
    MWindowEvent_Hide       = 'H',
    MWindowEvent_Resize     = 'R',
    MWindowEvent_Draw       = 'D',
    MWindowEvent_TouchBegin = 'B',
    MWindowEvent_TouchMove  = 'M',
    MWindowEvent_TouchEnd   = 'E',
    MWindowEvent_TextBox    = 'T',
    MWindowEvent_KeyDown    = 'K',
};

//------------------------------------------------------------------------------
//window:

//this type is used to mark the graphics size of the host ui.
typedef float _MPixel;

MFUNC_HOST void _MWindowSetPixelDensity(float density);

//window life cycle:

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

//host draw:

MFUNC_HOST int     _MWindowGraphCount();
MFUNC_HOST _MGraph _MWindowGraphType (int index);

MFUNC_HOST _MPixel  _MWindowTriangleGraphX0   (int index);
MFUNC_HOST _MPixel  _MWindowTriangleGraphY0   (int index);
MFUNC_HOST _MPixel  _MWindowTriangleGraphX1   (int index);
MFUNC_HOST _MPixel  _MWindowTriangleGraphY1   (int index);
MFUNC_HOST _MPixel  _MWindowTriangleGraphX2   (int index);
MFUNC_HOST _MPixel  _MWindowTriangleGraphY2   (int index);
MFUNC_HOST MColor   _MWindowTriangleGraphColor(int index);

MFUNC_HOST MImage  *_MWindowImageGraphObject  (int index);
MFUNC_HOST _MPixel  _MWindowImageGraphX       (int index);
MFUNC_HOST _MPixel  _MWindowImageGraphY       (int index);
MFUNC_HOST _MPixel  _MWindowImageGraphWidth   (int index);
MFUNC_HOST _MPixel  _MWindowImageGraphHeight  (int index);

MFUNC_HOST MString *_MWindowLabelGraphString  (int index);
MFUNC_HOST MColor   _MWindowLabelGraphColor   (int index);
MFUNC_HOST _MPixel  _MWindowLabelGraphFontSize(int index);
MFUNC_HOST MHAlign  _MWindowLabelGraphHAlign  (int index);
MFUNC_HOST MVAlign  _MWindowLabelGraphVAlign  (int index);
MFUNC_HOST _MPixel  _MWindowLabelGraphX       (int index);
MFUNC_HOST _MPixel  _MWindowLabelGraphY       (int index);
MFUNC_HOST _MPixel  _MWindowLabelGraphWidth   (int index);
MFUNC_HOST _MPixel  _MWindowLabelGraphHeight  (int index);

//host text box:

MFUNC_HOST bool     _MWindowTextBoxUpdated  ();
MFUNC_HOST bool     _MWindowTextBoxEnabled  ();
MFUNC_HOST MString *_MWindowTextBoxRawString();

//user listening:

MFUNC_EXPORT void MWindowAddListener(MLambda *listener) MFUNC_META(MWindowAddListener);

MFUNC_EXPORT MWindowEvent MWindowCurrentEvent() MFUNC_META(MWindowCurrentEvent);

MFUNC_EXPORT float MWindowWidth () MFUNC_META(MWindowWidth );
MFUNC_EXPORT float MWindowHeight() MFUNC_META(MWindowHeight);

MFUNC_EXPORT bool MWindowLoaded() MFUNC_META(MWindowLoaded);
MFUNC_EXPORT bool MWindowShown () MFUNC_META(MWindowShown );

MFUNC_EXPORT float MWindowTouchX() MFUNC_META(MWindowTouchX);
MFUNC_EXPORT float MWindowTouchY() MFUNC_META(MWindowTouchY);

MFUNC_EXPORT MKey MWindowActiveKey() MFUNC_META(MWindowActiveKey);

//user draw:

MFUNC_EXPORT void MWindowSelectString  (MString *string) MFUNC_META(MWindowSelectString  );
MFUNC_EXPORT void MWindowSelectImage   (MImage  *image ) MFUNC_META(MWindowSelectImage   );
MFUNC_EXPORT void MWindowSelectColor   (MColor   color ) MFUNC_META(MWindowSelectColor   );
MFUNC_EXPORT void MWindowSelectFontSize(float    size  ) MFUNC_META(MWindowSelectFontSize);
MFUNC_EXPORT void MWindowSelectHAlign  (MHAlign  align ) MFUNC_META(MWindowSelectHAlign  );
MFUNC_EXPORT void MWindowSelectVAlign  (MVAlign  align ) MFUNC_META(MWindowSelectVAlign  );

MFUNC_EXPORT void MWindowSelectPoint0(float x, float y) MFUNC_META(MWindowSelectPoint0);
MFUNC_EXPORT void MWindowSelectPoint1(float x, float y) MFUNC_META(MWindowSelectPoint1);
MFUNC_EXPORT void MWindowSelectPoint2(float x, float y) MFUNC_META(MWindowSelectPoint2);

MFUNC_EXPORT void MWindowDrawTriangle() MFUNC_META(MWindowDrawTriangle);
MFUNC_EXPORT void MWindowDrawImage   () MFUNC_META(MWindowDrawImage   );
MFUNC_EXPORT void MWindowDrawLabel   () MFUNC_META(MWindowDrawLabel   );

//text box control:

MFUNC_EXPORT void MWindowSetTextBoxUpdated  (bool     updated ) MFUNC_META(MWindowSetTextBoxUpdated  );
MFUNC_EXPORT void MWindowSetTextBoxEnabled  (bool     enabled ) MFUNC_META(MWindowSetTextBoxEnabled  );
MFUNC_EXPORT void MWindowSetTextBoxRawString(MString *original) MFUNC_META(MWindowSetTextBoxRawString);

MFUNC_EXPORT MString *MWindowTextBoxString() MFUNC_META(MWindowTextBoxString);
MFUNC_EXPORT bool     MWindowTextBoxEnter () MFUNC_META(MWindowTextBoxEnter );
