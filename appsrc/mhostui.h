#pragma once

#include "mexport.h"
#include "mtypes.h"

typedef int MKey;

const MKey MKey_Left  = 0x25;
const MKey MKey_Up    = 0x26;
const MKey MKey_Right = 0x27;
const MKey MKey_Down  = 0x28;
const MKey MKey_A     = 'A';
const MKey MKey_W     = 'W';
const MKey MKey_D     = 'D';
const MKey MKey_S     = 'S';

typedef int MAlign;

const MAlign MAlign_Left   = 1;
const MAlign MAlign_Center = 2;
const MAlign MAlign_Right  = 3;

typedef int MColor;

MColor MColorMake(float r, float g, float b, float a);

float MColorRed  (MColor color);
float MColorGreen(MColor color);
float MColorBlue (MColor color);
float MColorAlpha(MColor color);

MEXPORT(MColorMake )
MEXPORT(MColorRed  )
MEXPORT(MColorGreen)
MEXPORT(MColorBlue )
MEXPORT(MColorAlpha)

extern "C" void MWindowOnLoad  ();
extern "C" void MWindowOnShow  ();
extern "C" void MWindowOnHide  ();
extern "C" void MWindowOnUnload();

extern "C" void MWindowOnResize(float width, float height);
extern "C" void MWindowOnUpdate();
extern "C" void MWindowOnDraw  ();

extern "C" void MWindowOnTouchBegin(float x, float y);
extern "C" void MWindowOnTouchMove (float x, float y);
extern "C" void MWindowOnTouchEnd  (float x, float y);

extern "C" void MWindowOnText(int textId, bool enter);

extern "C" void MWindowOnKey(MKey key);

extern "C" int  MWindowTriangleCount();
extern "C" void MWindowSelectTriangle(int index);

extern "C" int  MWindowLabelCount();
extern "C" void MWindowSelectLabel(int index);

extern "C" int  MWindowDirtyTextCount();
extern "C" void MWindowSelectDirtyText(int index);
extern "C" int  MWindowObsoleteTextCount();
extern "C" void MWindowSelectObsoleteText(int index);

extern "C" int      MWindowSelectedId    ();
extern "C" MColor   MWindowSelectedColor ();
extern "C" MString *MWindowSelectedString();
extern "C" MAlign   MWindowSelectedAlign ();
extern "C" float    MWindowSelectedX     ();
extern "C" float    MWindowSelectedY     ();
extern "C" float    MWindowSelectedWidth ();
extern "C" float    MWindowSelectedHeight();

typedef int MWindowEvent;

const MWindowEvent MWindowEvent_Load       = 'L';
const MWindowEvent MWindowEvent_Show       = 'S';
const MWindowEvent MWindowEvent_Hide       = 'H';
const MWindowEvent MWindowEvent_Unload     = 'U';
const MWindowEvent MWindowEvent_Resize     = 'R';
const MWindowEvent MWindowEvent_Update     = 'P';
const MWindowEvent MWindowEvent_Draw       = 'D';
const MWindowEvent MWindowEvent_TouchBegin = 'B';
const MWindowEvent MWindowEvent_TouchMove  = 'M';
const MWindowEvent MWindowEvent_TouchEnd   = 'E';
const MWindowEvent MWindowEvent_Key        = 'K';
const MWindowEvent MWindowEvent_Text       = 'T';

void MWindowAddListener(MLambda *listener);

float MWindowWidth    ();
float MWindowHeight   ();
float MWindowTouchX   ();
float MWindowTouchY   ();
int   MWindowFocusText();
MKey  MWindowActiveKey();

void MWindowSelectColor (MColor   color );
void MWindowSelectString(MString *string);
void MWindowSelectAlign (MAlign   align );

void MWindowPinPoint(float x, float y);
void MWindowAddPoint(float x, float y);

void MWindowDrawTriangle();
void MWindowDrawLabel();

int MWindowAddText();

void     MWindowRemoveText   (int textId);
void     MWindowSetTextOrigin(int textId, float x, float y);
void     MWindowSetTextSize  (int textId, float w, float h);
MString *MWindowTextString   (int textId);

MEXPORT(MWindowAddListener  )
MEXPORT(MWindowWidth        )
MEXPORT(MWindowHeight       )
MEXPORT(MWindowTouchX       )
MEXPORT(MWindowTouchY       )
MEXPORT(MWindowFocusText    )
MEXPORT(MWindowActiveKey    )
MEXPORT(MWindowSelectColor  )
MEXPORT(MWindowSelectString )
MEXPORT(MWindowSelectAlign  )
MEXPORT(MWindowPinPoint     )
MEXPORT(MWindowAddPoint     )
MEXPORT(MWindowDrawTriangle )
MEXPORT(MWindowDrawLabel    )
MEXPORT(MWindowAddText      )
MEXPORT(MWindowRemoveText   )
MEXPORT(MWindowSetTextOrigin)
MEXPORT(MWindowSetTextSize  )
MEXPORT(MWindowTextString   )
