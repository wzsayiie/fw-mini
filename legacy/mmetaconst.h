#pragma once

#include "mbasics.h"

void _MConstSetU8Chars (const char *name, const char     *value, const char *note);
void _MConstSetU16Chars(const char *name, const char16_t *value, const char *note);
void _MConstSetInt     (const char *name, int             value, const char *note);
void _MConstSetFloat   (const char *name, float           value, const char *note);

//traverse constants.
M_FUNC_EXPORT void MConstSelectFirst  () M_META(MConstSelectFirst  );
M_FUNC_EXPORT bool MConstSelectedValid() M_META(MConstSelectedValid);
M_FUNC_EXPORT void MConstSelectNext   () M_META(MConstSelectNext   );

//select a constant.
M_FUNC_EXPORT bool MConstSelect(const char *name) M_META(MConstSelect, "args:name");

//the meta information of selected constant.
M_FUNC_EXPORT MString *MConstSelectedName  () M_META(MConstSelectedName  );
M_FUNC_EXPORT MString *MConstSelectedNote  () M_META(MConstSelectedNote  );
M_FUNC_EXPORT MTypeId  MConstSelectedTypeId() M_META(MConstSelectedTypeId);
M_FUNC_EXPORT MString *MConstSelectedString() M_META(MConstSelectedString);
M_FUNC_EXPORT int      MConstSelectedInt   () M_META(MConstSelectedInt   );
M_FUNC_EXPORT float    MConstSelectedFloat () M_META(MConstSelectedFloat );
