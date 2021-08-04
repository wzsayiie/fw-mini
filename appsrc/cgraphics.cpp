#include "cgraphics.h"

//------------------------------------------------------------------------------
//CColor:

CColor::CColor(float red, float green, float blue, float alpha) {
    set(red, green, blue, alpha);
}

void CColor::set(float red, float green, float blue, float alpha) {
    mRed   = red  ;
    mGreen = green;
    mBlue  = blue ;
    mAlpha = alpha;
}

float CColor::red  () const { return mRed  ; }
float CColor::green() const { return mGreen; }
float CColor::blue () const { return mBlue ; }
float CColor::alpha() const { return mAlpha; }

MColor CColor::rgba() const {
    MColorPattern color;
    
    color.red   = (uint8_t)(mRed   * 255);
    color.green = (uint8_t)(mGreen * 255);
    color.blue  = (uint8_t)(mBlue  * 255);
    color.alpha = (uint8_t)(mAlpha * 255);
    
    return color.rgba;
}

bool CColor::isClear() const {
    return mAlpha == 0 || (mRed == 0 && mGreen == 0 && mBlue == 0);
}

const CColor CColor::blackColor(0.00f, 0.00f, 0.00f);
const CColor CColor::grayColor (0.90f, 0.90f, 0.90f);
const CColor CColor::whiteColor(1.00f, 1.00f, 1.00f);
const CColor CColor::redColor  (0.79f, 0.36f, 0.30f);
const CColor CColor::greenColor(0.12f, 0.63f, 0.52f);
const CColor CColor::blueColor (0.31f, 0.54f, 0.79f);

const CColor CColor::clearColor(0, 0, 0, 0);

//------------------------------------------------------------------------------
//CImage:

CImageRef CImage::fromData(const std::vector<uint8_t> &data) {
    MDataRef  imageData   = m_auto_release MDataCreate(&data[0], (int)data.size());
    MImageRef nativeImage = m_auto_release MCreateImage(imageData.get());

    if (nativeImage) {
        return CImageRef(new CImage(nativeImage));
    }
    return nullptr;
}

CImageRef CImage::fromBundle(const std::string &path) {
    MStringRef imagePath   = m_auto_release MStringCreateU8(path.c_str());
    MImageRef  nativeImage = m_auto_release MCreateImageFromBundle(imagePath.get());

    if (nativeImage) {
        return CImageRef(new CImage(nativeImage));
    }
    return nullptr;
}

CImageRef CImage::fromFile(const std::string &path) {
    MStringRef imagePath   = m_auto_release MStringCreateU8(path.c_str());
    MImageRef  nativeImage = m_auto_release MCreateImageFromFile(imagePath.get());

    if (nativeImage) {
        return CImageRef(new CImage(nativeImage));
    }
    return nullptr;
}

MImage *CImage::nativeImage() {
    return mNativeImage.get();
}

CImage::CImage(MImageRef nativeImage) {
    mNativeImage = nativeImage;
}

//------------------------------------------------------------------------------
//draw context:

void CContextSetOffset(float x, float y) {
    MContextSetOffset(x, y);
}

void CContextSelectColor(const CColor &color) {
    MContextSelectColor(color.rgba());
}

void CContextSelectImage(CImageRef image) {
    MContextSelectImage(image->nativeImage());
}

void CContextSelectString(const std::string &string) {
    MStringRef nativeString = m_auto_release MStringCreateU8(string.c_str());
    MContextSelectString(nativeString.get());
}

void CContextSelectFontSize(float size) {
    MContextSelectFontSize(size);
}

void CContextSelectHorizontalAlign(CHorizontalAlign align) {
    MContextSelectHAlign((MHAlign)align);
}

void CContextSelectVerticalAlign(CVerticalAlign align) {
    MContextSelectVAlign((MVAlign)align);
}

void CContextDrawRect(float x, float y, float width, float height) {
    MContextDrawRect(x, y, width, height);
}

void CContextDrawImage(float x, float y, float width, float height) {
    MContextDrawImage(x, y, width, height);
}

void CContextDrawString(float x, float y, float width, float height) {
    MContextDrawString(x, y, width, height);
}
