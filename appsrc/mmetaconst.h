#pragma once

#include "mbasics.h"

void _MConstSetU8Chars (const char *name, const char     *value);
void _MConstSetU16Chars(const char *name, const char16_t *value);
void _MConstSetInt     (const char *name, int             value);
void _MConstSetFloat   (const char *name, float           value);

//traverse constants.
MFUNC_EXPORT void MConstSelectFirst  () MFUNC_META(MConstSelectFirst  );
MFUNC_EXPORT bool MConstSelectedValid() MFUNC_META(MConstSelectedValid);
MFUNC_EXPORT void MConstSelectNext   () MFUNC_META(MConstSelectNext   );

//select a constant.
MFUNC_EXPORT bool MConstSelect(const char *name) MFUNC_META(MConstSelect);

//the meta information of selected constant.
MFUNC_EXPORT const char     *MConstSelectedName    () MFUNC_META(MConstSelectedName    );
MFUNC_EXPORT MTypeId         MConstSelectedTypeId  () MFUNC_META(MConstSelectedTypeId  );
MFUNC_EXPORT const char     *MConstSelectedU8Chars () MFUNC_META(MConstSelectedU8Chars );
MFUNC_EXPORT const char16_t *MConstSelectedU16Chars() MFUNC_META(MConstSelectedU16Chars);
MFUNC_EXPORT int             MConstSelectedInt     () MFUNC_META(MConstSelectedInt     );
MFUNC_EXPORT float           MConstSelectedFloat   () MFUNC_META(MConstSelectedFloat   );
