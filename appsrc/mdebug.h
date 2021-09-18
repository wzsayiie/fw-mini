#pragma once

#include "mformat.h"

void MDebug(_Printf_format_string_ const char *format, ...) __printflike(1, 2);

#define D(format, ...) MDebug(format, ##__VA_ARGS__)
