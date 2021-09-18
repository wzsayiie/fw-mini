#pragma once

#include "mhostui.h"

//"MContext" takes the upper left corner as the origin,
//the X axis is right and the Y axis is down.

M_FUNC_EXPORT void MContextSetOffset(float x, float y) MFUNC_META(MContextSetOffset);

M_FUNC_EXPORT void MContextSelectColor    (MColor   color ) MFUNC_META(MContextSelectColor    );
M_FUNC_EXPORT void MContextSelectImage    (MImage  *image ) MFUNC_META(MContextSelectImage    );
M_FUNC_EXPORT void MContextSelectString   (MString *string) MFUNC_META(MContextSelectString   );
M_FUNC_EXPORT void MContextSelectHAlign   (MHAlign  align ) MFUNC_META(MContextSelectHAlign   );
M_FUNC_EXPORT void MContextSelectVAlign   (MVAlign  align ) MFUNC_META(MContextSelectVAlign   );
M_FUNC_EXPORT void MContextSelectFontSize (float    size  ) MFUNC_META(MContextSelectFontSize );
M_FUNC_EXPORT void MContextSelectLineWidth(float    width ) MFUNC_META(MContextSelectLineWidth);

M_FUNC_EXPORT void MContextDrawTriangle0(float x0, float y0) MFUNC_META(MContextDrawTriangle0);
M_FUNC_EXPORT void MContextDrawTriangle1(float x1, float y1) MFUNC_META(MContextDrawTriangle1);
M_FUNC_EXPORT void MContextDrawTriangle2(float x2, float y2) MFUNC_META(MContextDrawTriangle2);

M_FUNC_EXPORT void MContextDrawFlatLine  (float x0, float y0, float x1, float y1) MFUNC_META(MContextDrawFlatLine  );
M_FUNC_EXPORT void MContextDrawSquareLine(float x0, float y0, float x1, float y1) MFUNC_META(MContextDrawSquareLine);
M_FUNC_EXPORT void MContextDrawRoundLine (float x0, float y0, float x1, float y1) MFUNC_META(MContextDrawRoundLine );

M_FUNC_EXPORT void MContextDrawRect   (float x, float y, float width, float height) MFUNC_META(MContextDrawRect   );
M_FUNC_EXPORT void MContextDrawEllipse(float x, float y, float width, float height) MFUNC_META(MContextDrawEllipse);
M_FUNC_EXPORT void MContextDrawImage  (float x, float y, float width, float height) MFUNC_META(MContextDrawImage  );
M_FUNC_EXPORT void MContextDrawString (float x, float y, float width, float height) MFUNC_META(MContextDrawString );
