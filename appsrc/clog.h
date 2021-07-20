#pragma once

#include "mdebug.h"

void CLog(_Printf_format_string_ const char *format, ...) __printflike(1, 2);

#define I(format, ...) CLog(format, ##__VA_ARGS__)
