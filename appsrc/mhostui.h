#pragma once

#include "mhostapi.h"

//------------------------------------------------------------------------------
//configuration:

const char     *const MWindowTitleU8Name  =  "Mini" M_META(MWindowTitleU8Name );
const char16_t *const MWindowTitleU16Name = u"Mini" M_META(MWindowTitleU16Name);

const float _MWindowDrawInterval = 0.1f;

//------------------------------------------------------------------------------
//graph:

typedef int _MGraph;

const _MGraph _MGraph_Triangle = 'T';
const _MGraph _MGraph_Label    = 'L';
const _MGraph _MGraph_Image    = 'I';

//------------------------------------------------------------------------------
//alignment:

typedef int MHAlign;

const MHAlign MHAlign_Left   = 'L' M_META(MHAlign_Left  );
const MHAlign MHAlign_Center = 'C' M_META(MHAlign_Center);
const MHAlign MHAlign_Right  = 'R' M_META(MHAlign_Right );

typedef int MVAlign;

const MVAlign MVAlign_Top    = 'T' M_META(MVAlign_Top   );
const MVAlign MVAlign_Center = 'C' M_META(MVAlign_Center);
const MVAlign MVAlign_Bottom = 'B' M_META(MVAlign_Bottom);

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

const MKey MKey_Back  = 0x08 M_META(MKey_Back );
const MKey MKey_Enter = 0x0D M_META(MKey_Enter);
const MKey MKey_Space = 0x20 M_META(MKey_Space);
const MKey MKey_Left  = 0x25 M_META(MKey_Left );
const MKey MKey_Up    = 0x26 M_META(MKey_Up   );
const MKey MKey_Right = 0x27 M_META(MKey_Right);
const MKey MKey_Down  = 0x28 M_META(MKey_Down );
const MKey MKey_A     = 'A'  M_META(MKey_A    );
const MKey MKey_W     = 'W'  M_META(MKey_W    );
const MKey MKey_D     = 'D'  M_META(MKey_D    );
const MKey MKey_S     = 'S'  M_META(MKey_S    );

//------------------------------------------------------------------------------
//window event:

typedef int MWindowEvent;

const MWindowEvent MWindowEvent_Load       = 'L' M_META(MWindowEvent_Load      );
const MWindowEvent MWindowEvent_Show       = 'S' M_META(MWindowEvent_Show      );
const MWindowEvent MWindowEvent_Hide       = 'H' M_META(MWindowEvent_Hide      );
const MWindowEvent MWindowEvent_Resize     = 'R' M_META(MWindowEvent_Resize    );
const MWindowEvent MWindowEvent_Draw       = 'D' M_META(MWindowEvent_Draw      );
const MWindowEvent MWindowEvent_TouchBegin = 'B' M_META(MWindowEvent_TouchBegin);
const MWindowEvent MWindowEvent_TouchMove  = 'M' M_META(MWindowEvent_TouchMove );
const MWindowEvent MWindowEvent_TouchEnd   = 'E' M_META(MWindowEvent_TouchEnd  );
const MWindowEvent MWindowEvent_TextBox    = 'T' M_META(MWindowEvent_TextBox   );
const MWindowEvent MWindowEvent_KeyDown    = 'K' M_META(MWindowEvent_KeyDown   );

//------------------------------------------------------------------------------
//window:

//this type is used to mark the graphics size of the host ui.
typedef float _MPixel;

M_FUNC_HOST void _MWindowSetPixelDensity(float density);

//window life cycle:

M_FUNC_HOST void _MWindowOnLoad();
M_FUNC_HOST void _MWindowOnShow();
M_FUNC_HOST void _MWindowOnHide();

M_FUNC_HOST void _MWindowOnResize(_MPixel width, _MPixel height);
M_FUNC_HOST void _MWindowOnDraw  ();

M_FUNC_HOST void _MWindowOnTouchBegin(_MPixel x, _MPixel y);
M_FUNC_HOST void _MWindowOnTouchMove (_MPixel x, _MPixel y);
M_FUNC_HOST void _MWindowOnTouchEnd  (_MPixel x, _MPixel y);
M_FUNC_HOST void _MWindowOnTextBox   (MString *string, bool enter);
M_FUNC_HOST void _MWindowOnKeyDown   (MKey key);

//host draw:

M_FUNC_HOST int     _MWindowGraphCount();
M_FUNC_HOST _MGraph _MWindowGraphType (int index);

M_FUNC_HOST _MPixel  _MWindowTriangleGraphX0   (int index);
M_FUNC_HOST _MPixel  _MWindowTriangleGraphY0   (int index);
M_FUNC_HOST _MPixel  _MWindowTriangleGraphX1   (int index);
M_FUNC_HOST _MPixel  _MWindowTriangleGraphY1   (int index);
M_FUNC_HOST _MPixel  _MWindowTriangleGraphX2   (int index);
M_FUNC_HOST _MPixel  _MWindowTriangleGraphY2   (int index);
M_FUNC_HOST MColor   _MWindowTriangleGraphColor(int index);

M_FUNC_HOST MImage  *_MWindowImageGraphObject  (int index);
M_FUNC_HOST _MPixel  _MWindowImageGraphX       (int index);
M_FUNC_HOST _MPixel  _MWindowImageGraphY       (int index);
M_FUNC_HOST _MPixel  _MWindowImageGraphWidth   (int index);
M_FUNC_HOST _MPixel  _MWindowImageGraphHeight  (int index);

M_FUNC_HOST MString *_MWindowLabelGraphString  (int index);
M_FUNC_HOST MColor   _MWindowLabelGraphColor   (int index);
M_FUNC_HOST _MPixel  _MWindowLabelGraphFontSize(int index);
M_FUNC_HOST MHAlign  _MWindowLabelGraphHAlign  (int index);
M_FUNC_HOST MVAlign  _MWindowLabelGraphVAlign  (int index);
M_FUNC_HOST _MPixel  _MWindowLabelGraphX       (int index);
M_FUNC_HOST _MPixel  _MWindowLabelGraphY       (int index);
M_FUNC_HOST _MPixel  _MWindowLabelGraphWidth   (int index);
M_FUNC_HOST _MPixel  _MWindowLabelGraphHeight  (int index);

//host text box:

M_FUNC_HOST bool     _MWindowTextBoxUpdated  ();
M_FUNC_HOST bool     _MWindowTextBoxEnabled  ();
M_FUNC_HOST MString *_MWindowTextBoxRawString();

//user listening:

M_FUNC_EXPORT void MWindowAddListener(MLambda *listener) M_META(MWindowAddListener);

M_FUNC_EXPORT MWindowEvent MWindowCurrentEvent() M_META(MWindowCurrentEvent);

M_FUNC_EXPORT float MWindowWidth () M_META(MWindowWidth );
M_FUNC_EXPORT float MWindowHeight() M_META(MWindowHeight);

M_FUNC_EXPORT bool MWindowLoaded() M_META(MWindowLoaded);
M_FUNC_EXPORT bool MWindowShown () M_META(MWindowShown );

M_FUNC_EXPORT float MWindowTouchX() M_META(MWindowTouchX);
M_FUNC_EXPORT float MWindowTouchY() M_META(MWindowTouchY);

M_FUNC_EXPORT MKey MWindowActiveKey() M_META(MWindowActiveKey);

//user draw:

M_FUNC_EXPORT void MWindowSelectString  (MString *string) M_META(MWindowSelectString  );
M_FUNC_EXPORT void MWindowSelectImage   (MImage  *image ) M_META(MWindowSelectImage   );
M_FUNC_EXPORT void MWindowSelectColor   (MColor   color ) M_META(MWindowSelectColor   );
M_FUNC_EXPORT void MWindowSelectFontSize(float    size  ) M_META(MWindowSelectFontSize);
M_FUNC_EXPORT void MWindowSelectHAlign  (MHAlign  align ) M_META(MWindowSelectHAlign  );
M_FUNC_EXPORT void MWindowSelectVAlign  (MVAlign  align ) M_META(MWindowSelectVAlign  );

M_FUNC_EXPORT void MWindowSelectPoint0(float x, float y) M_META(MWindowSelectPoint0);
M_FUNC_EXPORT void MWindowSelectPoint1(float x, float y) M_META(MWindowSelectPoint1);
M_FUNC_EXPORT void MWindowSelectPoint2(float x, float y) M_META(MWindowSelectPoint2);

M_FUNC_EXPORT void MWindowDrawTriangle() M_META(MWindowDrawTriangle);
M_FUNC_EXPORT void MWindowDrawImage   () M_META(MWindowDrawImage   );
M_FUNC_EXPORT void MWindowDrawLabel   () M_META(MWindowDrawLabel   );

//text box control:

M_FUNC_EXPORT void MWindowSetTextBoxUpdated  (bool     updated ) M_META(MWindowSetTextBoxUpdated  );
M_FUNC_EXPORT void MWindowSetTextBoxEnabled  (bool     enabled ) M_META(MWindowSetTextBoxEnabled  );
M_FUNC_EXPORT void MWindowSetTextBoxRawString(MString *original) M_META(MWindowSetTextBoxRawString);

M_FUNC_EXPORT MString *MWindowTextBoxString() M_META(MWindowTextBoxString);
M_FUNC_EXPORT bool     MWindowTextBoxEnter () M_META(MWindowTextBoxEnter );
