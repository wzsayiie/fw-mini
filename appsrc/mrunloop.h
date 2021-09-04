#pragma once

#include "mtypes.h"

MFUNC_EXPORT float MRunningSeconds() MFUNC_META(MRunningSeconds);

MFUNC_EXPORT void MRunAfterSeconds(float delay   , MLambda *task) MFUNC_META(MRunAfterSeconds);
MFUNC_EXPORT void MRunEverySeconds(float interval, MLambda *task) MFUNC_META(MRunEverySeconds);

MLambdaRef MRunAfterSeconds(float delay   , std::function<void ()> task);
MLambdaRef MRunEverySeconds(float interval, std::function<void ()> task);

MFUNC_EXPORT void MCancelTask(MLambda *task) MFUNC_META(MCancelTask);
