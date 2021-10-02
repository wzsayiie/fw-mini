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

float CColor::redComponent  () const { return pattern().red   / 255.f; }
float CColor::greenComponent() const { return pattern().green / 255.f; }
float CColor::blueComponent () const { return pattern().blue  / 255.f; }
float CColor::alphaComponent() const { return pattern().alpha / 255.f; }

MColor CColor::color() const {
    return mColor;
}

bool CColor::isClear() const {
    return pattern().alpha == 0;
}

const CColor CColor::blackColor    (MColor_BlackColor    );
const CColor CColor::darkGrayColor (MColor_DarkGrayColor );
const CColor CColor::grayColor     (MColor_GrayColor     );
const CColor CColor::lightGrayColor(MColor_LightGrayColor);
const CColor CColor::whiteColor    (MColor_WhiteColor    );
const CColor CColor::redColor      (MColor_RedColor      );
const CColor CColor::greenColor    (MColor_GreenColor    );
const CColor CColor::blueColor     (MColor_BlueColor     );
const CColor CColor::cyanColor     (MColor_CyanColor     );
const CColor CColor::yellowColor   (MColor_YellowColor   );
const CColor CColor::magentaColor  (MColor_MagentaColor  );
const CColor CColor::orangeColor   (MColor_OrangeColor   );
const CColor CColor::purpleColor   (MColor_PurpleColor   );
const CColor CColor::brownColor    (MColor_BrownColor    );
const CColor CColor::clearColor    (MColor_ClearColor    );

MColorPattern CColor::pattern() const {
    MColorPattern pattern;
    pattern.color = mColor;
    return pattern;
}

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

void CContextSelectColor(const CColor &color) {
    MContextSelectColor(color.color());
}

void CContextSelectImage(CImageRef image) {
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
