#pragma once

#include "mbasics.h"

M_FUNC_EXPORT float MTickSeconds() M_META(MTickSeconds);

M_FUNC_EXPORT void MRunAfterSeconds(float delay   , MLambda *task) M_META(MRunAfterSeconds, "args:delay,task");
M_FUNC_EXPORT void MRunEverySeconds(float interval, MLambda *task) M_META(MRunEverySeconds, "args:space,task");
M_FUNC_EXPORT void MCancelTask     (MLambda *task)                 M_META(MCancelTask     , "args:task");

MLambdaRef MRunAfterSeconds(float delay   , std::function<void ()> task);
MLambdaRef MRunEverySeconds(float interval, std::function<void ()> task);
