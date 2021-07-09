#pragma once

#include "mhostui.h"
#include "mtypeex.h"

class CColor {
    
public:
    CColor(float red, float green, float blue, float alpha = 1);
    
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

typedef std::shared_ptr<class CImage> CImageRef;

class CImage {

public:
    CImage(const std::vector<uint8_t> &data);
    CImage(const std::string &path);

    MImage *nativeImage();

private:
    MImageRef mNativeImage;
};

void CContextSetOffset(float x, float y);

void CContextSelectColor(const CColor &color);
void CContextSelectImage(CImageRef image);

void CContextDrawEllipse(float x, float y, float width, float height);
void CContextDrawRect   (float x, float y, float width, float height);
void CContextDrawImage  (float x, float y, float width, float height);
