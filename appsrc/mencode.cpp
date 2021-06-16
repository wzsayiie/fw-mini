#include "mencode.h"

int MReadU8(const void *begin, const void *end, char32_t *outChar) {
    return 0;
}

int MReadU16(const void *begin, const void *end, char32_t *outChar) {
    return 0;
}

int MReadGBK(const void *begin, const void *end, char32_t *outChar) {
    return 0;
}

int NWriteU8(void *dst, char32_t chr) {
    return 0;
}

int NWriteU16(void *dst, char32_t chr) {
    return 0;
}

int NWriteGBK(void *dst, char32_t chr) {
    return 0;
}

std::string MU8stringFromU16s(const char16_t *u16chars) {
    return "";
}

std::u16string MU16stringFromU8s(const char *u8chars) {
    return u"";
}
