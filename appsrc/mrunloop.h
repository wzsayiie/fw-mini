#pragma once

#include "mbasics.h"

M_FUNC_EXPORT float MRunningSeconds() M_META(MRunningSeconds);

M_FUNC_EXPORT void MRunAfterSeconds(float delay   , MLambda *task) M_META(MRunAfterSeconds);
M_FUNC_EXPORT void MRunEverySeconds(float interval, MLambda *task) M_META(MRunEverySeconds);

MLambdaRef MRunAfterSeconds(float delay   , std::function<void ()> task);
MLambdaRef MRunEverySeconds(float interval, std::function<void ()> task);

M_FUNC_EXPORT void MCancelTask(MLambda *task) M_META(MCancelTask);
