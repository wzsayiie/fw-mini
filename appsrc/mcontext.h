#pragma once

#include "mhostui.h"

//"MContext" takes the upper left corner as the origin,
//the X axis is right and the Y axis is down.

MFUNC_EXPORT void MContextSetOffset(float x, float y) MFUNC_META(MContextSetOffset);

MFUNC_EXPORT void MContextSelectColor    (MColor   color ) MFUNC_META(MContextSelectColor    );
MFUNC_EXPORT void MContextSelectImage    (MImage  *image ) MFUNC_META(MContextSelectImage    );
MFUNC_EXPORT void MContextSelectString   (MString *string) MFUNC_META(MContextSelectString   );
MFUNC_EXPORT void MContextSelectHAlign   (MHAlign  align ) MFUNC_META(MContextSelectHAlign   );
MFUNC_EXPORT void MContextSelectVAlign   (MVAlign  align ) MFUNC_META(MContextSelectVAlign   );
MFUNC_EXPORT void MContextSelectFontSize (float    size  ) MFUNC_META(MContextSelectFontSize );
MFUNC_EXPORT void MContextSelectLineWidth(float    width ) MFUNC_META(MContextSelectLineWidth);

MFUNC_EXPORT void MContextDrawTriangle0(float x0, float y0) MFUNC_META(MContextDrawTriangle0);
MFUNC_EXPORT void MContextDrawTriangle1(float x1, float y1) MFUNC_META(MContextDrawTriangle1);
MFUNC_EXPORT void MContextDrawTriangle2(float x2, float y2) MFUNC_META(MContextDrawTriangle2);

MFUNC_EXPORT void MContextDrawFlatLine  (float x0, float y0, float x1, float y1) MFUNC_META(MContextDrawFlatLine  );
MFUNC_EXPORT void MContextDrawSquareLine(float x0, float y0, float x1, float y1) MFUNC_META(MContextDrawSquareLine);
MFUNC_EXPORT void MContextDrawRoundLine (float x0, float y0, float x1, float y1) MFUNC_META(MContextDrawRoundLine );

MFUNC_EXPORT void MContextDrawRect   (float x, float y, float width, float height) MFUNC_META(MContextDrawRect   );
MFUNC_EXPORT void MContextDrawEllipse(float x, float y, float width, float height) MFUNC_META(MContextDrawEllipse);
MFUNC_EXPORT void MContextDrawImage  (float x, float y, float width, float height) MFUNC_META(MContextDrawImage  );
MFUNC_EXPORT void MContextDrawString (float x, float y, float width, float height) MFUNC_META(MContextDrawString );
