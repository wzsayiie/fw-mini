#pragma once

#include <string>
#include "mhostui.h"

class CColor {
    
public:
    CColor(float red, float green, float blue, float alpha = 1);
    CColor();
    
    void set(float red, float green, float blue, float alpha = 1);
    
    float red  () const;
    float green() const;
    float blue () const;
    float alpha() const;
    
    MColor rgba() const;
    
private:
    float mRed   = 1;
    float mGreen = 1;
    float mBlue  = 1;
    float mAlpha = 1;
};

void CContextSetOffset(float x, float y);

void CContextSelectColor(const CColor &color);

void CContextDrawEllipse(float x, float y, float width, float height);
void CContextDrawRect   (float x, float y, float width, float height);
