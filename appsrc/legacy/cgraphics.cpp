#include "cgraphics.h"

//------------------------------------------------------------------------------
//CColor:

CColor::CColor(float red, float green, float blue, float alpha) {
    MColorPattern pattern;
    
    pattern.red   = (uint8_t)(red   * 255.f);
    pattern.green = (uint8_t)(green * 255.f);
    pattern.blue  = (uint8_t)(blue  * 255.f);
    pattern.alpha = (uint8_t)(alpha * 255.f);
    
    mColor = pattern.color;
}

CColor::CColor(MColor color) {
    mColor = color;
}

CColor::CColor() {
    mColor = 0;
}

float CColor::redComponent  () const { return (float)pattern().red   / 255.f; }
float CColor::greenComponent() const { return (float)pattern().green / 255.f; }
float CColor::blueComponent () const { return (float)pattern().blue  / 255.f; }
float CColor::alphaComponent() const { return (float)pattern().alpha / 255.f; }

MColor CColor::color() const {
    return mColor;
}

bool CColor::isClear() const {
    return pattern().alpha == 0;
}

const CColor CColor::BlackColor     = MColor_BlackColor    ;
const CColor CColor::DarkGrayColor  = MColor_DarkGrayColor ;
const CColor CColor::GrayColor      = MColor_GrayColor     ;
const CColor CColor::LightGrayColor = MColor_LightGrayColor;
const CColor CColor::WhiteColor     = MColor_WhiteColor    ;
const CColor CColor::RedColor       = MColor_RedColor      ;
const CColor CColor::GreenColor     = MColor_GreenColor    ;
const CColor CColor::BlueColor      = MColor_BlueColor     ;
const CColor CColor::CyanColor      = MColor_CyanColor     ;
const CColor CColor::YellowColor    = MColor_YellowColor   ;
const CColor CColor::MagentaColor   = MColor_MagentaColor  ;
const CColor CColor::OrangeColor    = MColor_OrangeColor   ;
const CColor CColor::PurpleColor    = MColor_PurpleColor   ;
const CColor CColor::BrownColor     = MColor_BrownColor    ;
const CColor CColor::ClearColor     = MColor_ClearColor    ;

MColorPattern CColor::pattern() const {
    MColorPattern pattern;
    pattern.color = mColor;
    return pattern;
}

//------------------------------------------------------------------------------
//CImage:

CImageRef CImage::reference(const MImageRef &nativeImage) {
    if (nativeImage) {
        return CImageRef(new CImage(nativeImage));
    }
    return nullptr;
}

CImageRef CImage::fromBitmap(const std::vector<uint8_t> &data, int width, int height) {
    MDataRef  imageData   = m_auto_release MDataCopy(&data[0], (int)data.size());
    MImageRef nativeImage = m_auto_release MCreateBitmapImage(imageData.get(), width, height);

    return reference(nativeImage);
}

CImageRef CImage::fromData(const std::vector<uint8_t> &data) {
    MDataRef  imageData   = m_auto_release MDataCopy(&data[0], (int)data.size());
    MImageRef nativeImage = m_auto_release MCreateImage(imageData.get());

    return reference(nativeImage);
}

CImageRef CImage::fromBundle(const std::string &path) {
    MStringRef imagePath   = m_auto_release MStringCreateU8(path.c_str());
    MImageRef  nativeImage = m_auto_release MCreateImageFromBundle(imagePath.get());

    return reference(nativeImage);
}

CImageRef CImage::fromFile(const std::string &path) {
    MStringRef imagePath   = m_auto_release MStringCreateU8(path.c_str());
    MImageRef  nativeImage = m_auto_release MCreateImageFromFile(imagePath.get());

    return reference(nativeImage);
}

std::vector<uint8_t> CImage::bitmap() {
    MDataRef data = m_auto_release MCopyImageBitmap(mNativeImage.get());
    return std::vector<uint8_t>(MDataBytes(data.get()), MDataEnd(data.get()));
}

int CImage::width () { return MImagePixelWidth (mNativeImage.get()); }
int CImage::height() { return MImagePixelHeight(mNativeImage.get()); }

MImage *CImage::nativeImage() {
    return mNativeImage.get();
}

CImage::CImage(const MImageRef &nativeImage) {
    mNativeImage = nativeImage;
}

//------------------------------------------------------------------------------
//draw context:

void CContextSelectColor(const CColor &color) {
    MContextSelectColor(color.color());
}

void CContextSelectImage(const CImageRef &image) {
    MContextSelectImage(image->nativeImage());
}

void CContextSelectString(const std::string &string) {
    MStringRef nativeString = m_auto_release MStringCreateU8(string.c_str());
    MContextSelectString(nativeString.get());
}

void CContextSelectHorizontalAlign(CHorizontalAlign align) {
    MContextSelectHAlign((MHAlign)align);
}

void CContextSelectVerticalAlign(CVerticalAlign align) {
    MContextSelectVAlign((MVAlign)align);
}
