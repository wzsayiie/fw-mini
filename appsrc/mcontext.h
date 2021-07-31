#pragma once

#include "mhostui.h"

MFUNC_EXPORT void MContextSetOffset(float x, float y) MFUNC_META(MContextSetOffset);

MFUNC_EXPORT void MContextSelectColor   (MColor   color ) MFUNC_META(MContextSelectColor   );
MFUNC_EXPORT void MContextSelectImage   (MImage  *image ) MFUNC_META(MContextSelectImage   );
MFUNC_EXPORT void MContextSelectString  (MString *string) MFUNC_META(MContextSelectString  );
MFUNC_EXPORT void MContextSelectFontSize(float    size  ) MFUNC_META(MContextSelectFontSize);
MFUNC_EXPORT void MContextSelectHAlign  (MHAlign  align ) MFUNC_META(MContextSelectHAlign  );
MFUNC_EXPORT void MContextSelectVAlign  (MVAlign  align ) MFUNC_META(MContextSelectVAlign  );

MFUNC_EXPORT void MContextDrawRect  (float x, float y, float width, float height) MFUNC_META(MContextDrawRect  );
MFUNC_EXPORT void MContextDrawImage (float x, float y, float width, float height) MFUNC_META(MContextDrawImage );
MFUNC_EXPORT void MContextDrawString(float x, float y, float width, float height) MFUNC_META(MContextDrawString);
