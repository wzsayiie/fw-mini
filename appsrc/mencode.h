#pragma once

#include <string>

int MReadU8 (const void *begin, const void *end, char32_t *outChar);
int MReadU16(const void *begin, const void *end, char32_t *outChar);
int MReadGBK(const void *begin, const void *end, char32_t *outChar);

int NWriteU8 (void *dst, char32_t chr);
int NWriteU16(void *dst, char32_t chr);
int NWriteGBK(void *dst, char32_t chr);

std::string    MU8StringFromU16(const char16_t *u16chars);
std::u16string MU16StringFromU8(const char     *u8chars );
