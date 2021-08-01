#pragma once

#include "mtypes.h"

MFUNC_EXPORT void MRunAfterSeconds(float delay   , MLambda *task) MFUNC_META(MRunAfterSeconds);
MFUNC_EXPORT void MRunEverySeconds(float interval, MLambda *task) MFUNC_META(MRunEverySeconds);

MFUNC_EXPORT void MCancelTask(MLambda *task) MFUNC_META(MCancelTask);
