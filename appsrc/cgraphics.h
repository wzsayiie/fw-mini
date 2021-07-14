#pragma once

#include "mhostui.h"
#include "mtypeex.h"

//------------------------------------------------------------------------------
//CColor:

class CColor {
    
public:
    CColor(float red, float green, float blue, float alpha = 1);
    
    void set(float red, float green, float blue, float alpha = 1);
    
    float red  () const;
    float green() const;
    float blue () const;
    float alpha() const;
    
    MColor rgba() const;

    static const CColor blackColor;
    static const CColor grayColor;
    static const CColor whiteColor;
    static const CColor redColor;
    static const CColor greenColor;
    static const CColor blueColor;
    static const CColor clearColor;
    
private:
    float mRed   = 0;
    float mGreen = 0;
    float mBlue  = 0;
    float mAlpha = 0;
};

//------------------------------------------------------------------------------
//CImage:

typedef std::shared_ptr<class CImage> CImageRef;

class CImage {

public:
    CImage(const std::vector<uint8_t> &data);
    CImage(const std::string &path);

    MImage *nativeImage();

private:
    MImageRef mNativeImage;
};

//------------------------------------------------------------------------------
//draw context:

void CContextSetOffset(float x, float y);

void CContextSelectColor(const CColor &color);
void CContextSelectImage(CImageRef image);

void CContextDrawEllipse(float x, float y, float width, float height);
void CContextDrawRect   (float x, float y, float width, float height);
void CContextDrawImage  (float x, float y, float width, float height);
