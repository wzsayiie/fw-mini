#pragma once

#include "mhostui.h"

//"MContext" takes the upper left corner as the origin,
//the X axis is right and the Y axis is down.

M_FUNC_EXPORT void MContextSetOffset(float x, float y) M_META(MContextSetOffset, "args:x,y");

M_FUNC_EXPORT void MContextSelectColor    (MColor   color ) M_META(MContextSelectColor    , "args:color");
M_FUNC_EXPORT void MContextSelectImage    (MImage  *image ) M_META(MContextSelectImage    , "args:image");
M_FUNC_EXPORT void MContextSelectString   (MString *string) M_META(MContextSelectString   , "args:str"  );
M_FUNC_EXPORT void MContextSelectHAlign   (MHAlign  align ) M_META(MContextSelectHAlign   , "args:align");
M_FUNC_EXPORT void MContextSelectVAlign   (MVAlign  align ) M_META(MContextSelectVAlign   , "args:align");
M_FUNC_EXPORT void MContextSelectFontSize (float    size  ) M_META(MContextSelectFontSize , "args:size" );
M_FUNC_EXPORT void MContextSelectLineWidth(float    width ) M_META(MContextSelectLineWidth, "args:width");

M_FUNC_EXPORT void MContextDrawTriangle0(float x0, float y0) M_META(MContextDrawTriangle0, "args:x0,y0");
M_FUNC_EXPORT void MContextDrawTriangle1(float x1, float y1) M_META(MContextDrawTriangle1, "args:x1,y1");
M_FUNC_EXPORT void MContextDrawTriangle2(float x2, float y2) M_META(MContextDrawTriangle2, "args:x2,y2");

M_FUNC_EXPORT void MContextDrawFlatLine  (float x0, float y0, float x1, float y1) M_META(MContextDrawFlatLine  , "args:x0,y0,x1,y1");
M_FUNC_EXPORT void MContextDrawSquareLine(float x0, float y0, float x1, float y1) M_META(MContextDrawSquareLine, "args:x0,y0,x1,y1");
M_FUNC_EXPORT void MContextDrawRoundLine (float x0, float y0, float x1, float y1) M_META(MContextDrawRoundLine , "args:x0,y0,x1,y1");

M_FUNC_EXPORT void MContextDrawRect   (float x, float y, float width, float height) M_META(MContextDrawRect   , "args:x,y,w,h");
M_FUNC_EXPORT void MContextDrawEllipse(float x, float y, float width, float height) M_META(MContextDrawEllipse, "args:x,y,w,h");
M_FUNC_EXPORT void MContextDrawImage  (float x, float y, float width, float height) M_META(MContextDrawImage  , "args:x,y,w,h");
M_FUNC_EXPORT void MContextDrawString (float x, float y, float width, float height) M_META(MContextDrawString , "args:x,y,w,h");
