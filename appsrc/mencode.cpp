#include "mencode.h"

int MReadU8(const char *begin, const char *end, char32_t *outCode) {
    //utf-8 first byte:
    //
    // 1 byte  occupied,  7 valid bits: 0xxx'xxxx
    // 2 bytes occupied, 11 valid bits: 110x'xxxx 10xx xxxx
    // 3 bytes occupied, 16 valid bits: 1110'xxxx 10xx xxxx ...
    // 4 bytes occupied, 21 valid bits: 1111'0xxx 10xx xxxx ...
    //
    int count = 0;
    if /**/ ((*begin & 0x80) == 0x00) { count = 1; *outCode = *begin & 0x7F; }
    else if ((*begin & 0xE0) == 0xC0) { count = 2; *outCode = *begin & 0x1F; }
    else if ((*begin & 0xF0) == 0xE0) { count = 3; *outCode = *begin & 0x0F; }
    else if ((*begin & 0xF8) == 0xF0) { count = 4; *outCode = *begin & 0x07; }

    if (count == 0 || begin + count > end) {
        //abnormal byte count.
        *outCode = 0;
        return 0;
    }

    //followed bytes: 10xx'xxxx
    for (const char *it = begin + 1; it < begin + count; ++it) {
        if ((*it & 0xC0) != 0x80) {
            //abnormal byte value.
            *outCode = 0;
            return 0;
        }
        *outCode <<= 6;
        *outCode |= (*it & 0x3F);
    }

    return count;
}

int MReadU16(const char16_t *begin, const char16_t *end, char32_t *outCode) {
    //utf-16 surrogate pair, 4 bytes:
    //
    //  1st 2 bytes: 1101 10 xx,xxxx xxxx
    //  2nd 2 bytes: 1101 11 xx,xxxx xxxx
    //
    if (/*  begin + 1 <= end */ 0xD800 <= begin[0] && begin[0] <= 0xDBff) {
        if (begin + 2 <= end && 0xDC00 <= begin[1] && begin[1] <= 0xDFff) {

            *outCode = (char32_t)(begin[0] & 0x03ff);
            *outCode = (char32_t)(begin[1] & 0x03ff) | (*outCode << 10);
            *outCode += 0x010000;

            return 2;
        } else {
            //abnormal bytes value.
            *outCode = 0;
            return 0;
        }
    }
    //BMP character, 2 bytes.
    else {
        *outCode = *begin;
        return 1;
    }
}

int MReadGBK(const uint8_t *begin, const uint8_t *end, uint16_t *outCode) {
    return 0;
}

int MWriteU8(char *dst, char32_t code) {
    if (code <= 0x7F) {
        //up to 7 bits, occupy 1 byte. 0xxx'xxxx.
        dst[0] = (int8_t)code;

        return 1;
    }
    if (code <= 0x07ff) {
        //up to 11 bits, occupy 2 bytes. 110x'xxxx 10xx xxxx.
        dst[0] = ((code >> 6) & 0x1F) | 0xC0;
        dst[1] = ((code     ) & 0x3F) | 0x80;

        return 2;
    }
    if (code <= 0xFFff) {
        //up to 16 bits, occupy 3 bytes. 1110'xxxx 10xx xxxx ... .
        dst[0] = ((code >> 12) & 0x0F) | 0xE0;
        dst[1] = ((code >>  6) & 0x3F) | 0x80;
        dst[2] = ((code      ) & 0x3F) | 0x80;

        return 3;
    }
    if (code <= 0x10ffFF) {
        //up to 21 bits, occupy 4 bytes. 1111'0xxx 10xx xxxx ... .
        dst[0] = ((code >> 18) & 0x07) | 0xF0;
        dst[1] = ((code >> 12) & 0x3F) | 0x80;
        dst[2] = ((code >>  6) & 0x3F) | 0x80;
        dst[3] = ((code      ) & 0x3F) | 0x80;

        return 4;
    }

    return 0;
}

int MWriteU16(char16_t *dst, char32_t code) {
    //utf-16 surrogate pair, 4 bytes:
    //
    //  1st 2 bytes: 1101 10 xx,xxxx xxxx
    //  2nd 2 bytes: 1101 11 xx,xxxx xxxx
    //
    if (code > 0xFFff) {

        //the biggest code-point of unicode is 0x10ffFF (the last of 16st plane),
        //when a code-point subtracted from 0x010000, the remaining value will not exceed 20 bit.
        code -= 0x010000;
        dst[0] = ((code >> 10) & 0x03ff) | 0xD800;
        dst[1] = ((code      ) & 0x03ff) | 0xDC00;

        return 2;
    }
    //BMP character, 2 bytes.
    else {
        dst[0] = (char16_t)code;
        return 1;
    }
}

int MWriteGBK(uint8_t *dst, uint16_t code) {
    return 0;
}

std::string MU8StringFromU16(const char16_t *u16chars) {
    std::string u8string;

    while (true) {
        char32_t code = 0;
        int count = MReadU16(u16chars, u16chars + 2, &code);
        if (count == 0 || code == '\0') {
            break;
        }
        u16chars += count;

        char buffer[5] = {0};
        MWriteU8(buffer, code);
        u8string.append(buffer);
    }
    
    return u8string;
}

std::u16string MU16StringFromU8(const char *u8chars ) {
    std::u16string u16string;

    while (true) {
        char32_t code = 0;
        int count = MReadU8(u8chars, u8chars + 4, &code);
        if (count == 0 || code == '\0') {
            break;
        }
        u8chars += count;

        char16_t buffer[3] = {0};
        MWriteU16(buffer, code);
        u16string.append(buffer);
    }

    return u16string;
}
