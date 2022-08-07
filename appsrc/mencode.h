#pragma once

#include "dash.h"

int MReadU8 (const char     *begin, const char     *end, char32_t *outCode);
int MReadU16(const char16_t *begin, const char16_t *end, char32_t *outCode);

int MWriteU8 (char     *dst, char32_t code);
int MWriteU16(char16_t *dst, char32_t code);

std::string    MU8StringFromU16(const char16_t *u16chars);
std::u16string MU16StringFromU8(const char     *u8chars );
