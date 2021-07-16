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

MEXPORT(MContextSetOffset     )
MEXPORT(MContextSelectColor   )
MEXPORT(MContextSelectImage   )
MEXPORT(MContextSelectString  )
MEXPORT(MContextSelectFontSize)
MEXPORT(MContextSelectHAlign  )
MEXPORT(MContextSelectVAlign  )
MEXPORT(MContextDrawRect      )
MEXPORT(MContextDrawImage     )
MEXPORT(MContextDrawString    )
