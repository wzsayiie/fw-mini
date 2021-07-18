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

    bool isClear() const;

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
    static CImageRef fromData  (const std::vector<uint8_t> &data);
    static CImageRef fromBundle(const std::string &path);
    static CImageRef fromFile  (const std::string &path);

    MImage *nativeImage();

private:
    static CImageRef decodeData(MDataRef data);
    
    MImageRef mNativeImage;
};

//------------------------------------------------------------------------------
//content alignment:

enum class CHorizontalAlign {
    None   = 0,
    Left   = MHAlign_Left  ,
    Center = MHAlign_Center,
    Right  = MHAlign_Right ,
};

enum class CVerticalAlign {
    None   = 0,
    Top    = MVAlign_Top   ,
    Center = MVAlign_Center,
    Bottom = MVAlign_Bottom,
};

//------------------------------------------------------------------------------
//draw context:

void CContextSetOffset(float x, float y);

void CContextSelectColor   (const CColor &color);
void CContextSelectImage   (CImageRef image);
void CContextSelectString  (const std::string &string);
void CContextSelectFontSize(float size);

void CContextSelectHorizontalAlign(CHorizontalAlign align);
void CContextSelectVerticalAlign  (CVerticalAlign   align);

void CContextDrawRect  (float x, float y, float width, float height);
void CContextDrawImage (float x, float y, float width, float height);
void CContextDrawString(float x, float y, float width, float height);
