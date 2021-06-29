#include "cgraphics.h"

CColor::CColor(float red, float green, float blue, float alpha) {
    set(red, blue, green, alpha);
}

CColor::CColor(const char *string) {
    set(string);
}

CColor::CColor() {
}

void CColor::set(float red, float green, float blue, float alpha) {
    mRed   = red  ;
    mGreen = green;
    mBlue  = blue ;
    mAlpha = alpha;
}

static int HexFromString(const char *string) {
    int n = 0;
    for (int i = 0; i < 8; ++i) {
        char c = string[i];
        
        if      ('0' <= c && c <= '9') { n = n * 16 + c - '0'     ; }
        else if ('A' <= c && c <= 'F') { n = n * 16 + c - 'A' + 10; }
        else if ('a' <= c && c <= 'f') { n = n * 16 + c - 'a' + 10; }
        else {
            break;
        }
    }
    return n;
}

void CColor::set(const char *string) {
    MColorPattern color = {0};
    if (string) {
        if (*string == '#') {
            string += 1;
        }
        color.rgba = HexFromString(string);
    }
    
    mRed   = color.rgba  / 255.f;
    mGreen = color.green / 255.f;
    mBlue  = color.blue  / 255.f;
    mAlpha = color.alpha / 255.f;
}

float CColor::red  () const { return mRed  ; }
float CColor::green() const { return mGreen; }
float CColor::blue () const { return mBlue ; }
float CColor::alpha() const { return mAlpha; }

MColor CColor::rgba() const {
    MColorPattern color = {0};
    
    color.red   = (uint8_t)(mRed   * 255);
    color.green = (uint8_t)(mGreen * 255);
    color.blue  = (uint8_t)(mBlue  * 255);
    color.alpha = (uint8_t)(mAlpha * 255);
    
    return color.rgba;
}
