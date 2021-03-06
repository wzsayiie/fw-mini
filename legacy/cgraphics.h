#pragma once

#include "minikit.h"

//------------------------------------------------------------------------------
//CColor:

class CColor {
    
public:
    CColor(float red, float green, float blue, float alpha = 1);
    CColor(MColor color);
    CColor();
    
    float redComponent  () const;
    float greenComponent() const;
    float blueComponent () const;
    float alphaComponent() const;
    
    MColor color() const;
    bool isClear() const;

    static const CColor BlackColor    ;
    static const CColor DarkGrayColor ;
    static const CColor GrayColor     ;
    static const CColor LightGrayColor;
    static const CColor WhiteColor    ;
    static const CColor RedColor      ;
    static const CColor GreenColor    ;
    static const CColor BlueColor     ;
    static const CColor CyanColor     ;
    static const CColor YellowColor   ;
    static const CColor MagentaColor  ;
    static const CColor OrangeColor   ;
    static const CColor PurpleColor   ;
    static const CColor BrownColor    ;
    static const CColor ClearColor    ;
    
private:
    MColorPattern pattern() const;
    
    MColor mColor;
};

//------------------------------------------------------------------------------
//CImage:

def_class(CImage) {

public:
    static CImageRef reference (const MImageRef &nativeImage);
    static CImageRef fromBitmap(const std::vector<uint8_t> &data, int width, int height);
    static CImageRef fromData  (const std::vector<uint8_t> &data);
    static CImageRef fromBundle(const std::string &path);
    static CImageRef fromFile  (const std::string &path);

    std::vector<uint8_t> bitmap();

    int width ();
    int height();

    MImage *nativeImage();

private:
    CImage(const MImageRef &nativeImage);
    
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

//"CContext" takes the upper left corner as the origin,
//the X axis is right and the Y axis is down.

void CContextSelectColor (const CColor      &color );
void CContextSelectImage (const CImageRef   &image );
void CContextSelectString(const std::string &string);

void CContextSelectHorizontalAlign(CHorizontalAlign align);
void CContextSelectVerticalAlign  (CVerticalAlign   align);

inline void CContextPushClip(float x, float y, float w, float h) { MContextPushClip(x, y, w, h); }
inline void CContextPopClip() { MContextPopClip(); }

inline void CContextSetDrawOffset(float x, float y) { MContextSetDrawOffset(x, y); }

inline void CContextSelectFontSize (float s) { MContextSelectFontSize (s); }
inline void CContextSelectLineWidth(float w) { MContextSelectLineWidth(w); }

inline void CContextDrawTriangle0(float x0, float y0) { MContextDrawTriangle0(x0, y0); }
inline void CContextDrawTriangle1(float x1, float y1) { MContextDrawTriangle1(x1, y1); }
inline void CContextDrawTriangle2(float x2, float y2) { MContextDrawTriangle2(x2, y2); }

inline void CContextDrawFlatLine  (float x0, float y0, float x1, float y1) { MContextDrawFlatLine  (x0, y0, x1, x1); }
inline void CContextDrawSquareLine(float x0, float y0, float x1, float y1) { MContextDrawSquareLine(x0, y0, x1, x1); }
inline void CContextDrawRoundLine (float x0, float y0, float x1, float y1) { MContextDrawRoundLine (x0, y0, x1, x1); }

inline void CContextDrawRect   (float x, float y, float w, float h) { MContextDrawRect   (x, y, w, h); }
inline void CContextDrawEllipse(float x, float y, float w, float h) { MContextDrawEllipse(x, y, w, h); }
inline void CContextDrawImage  (float x, float y, float w, float h) { MContextDrawImage  (x, y, w, h); }
inline void CContextDrawString (float x, float y, float w, float h) { MContextDrawString (x, y, w, h); }
