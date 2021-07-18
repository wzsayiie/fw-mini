#pragma once

#include "mhostui.h"

void MContextSetOffset(float x, float y);

void MContextSelectColor   (MColor   color );
void MContextSelectImage   (MImage  *image );
void MContextSelectString  (MString *string);
void MContextSelectFontSize(float    size  );
void MContextSelectHAlign  (MHAlign  align );
void MContextSelectVAlign  (MVAlign  align );

void MContextDrawRect  (float x, float y, float width, float height);
void MContextDrawImage (float x, float y, float width, float height);
void MContextDrawString(float x, float y, float width, float height);

MFUNC_EXPORT(MContextSetOffset     )
MFUNC_EXPORT(MContextSelectColor   )
MFUNC_EXPORT(MContextSelectImage   )
MFUNC_EXPORT(MContextSelectString  )
MFUNC_EXPORT(MContextSelectFontSize)
MFUNC_EXPORT(MContextSelectHAlign  )
MFUNC_EXPORT(MContextSelectVAlign  )
MFUNC_EXPORT(MContextDrawRect      )
MFUNC_EXPORT(MContextDrawImage     )
MFUNC_EXPORT(MContextDrawString    )
