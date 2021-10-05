#pragma once

#include "cprime.h"

//------------------------------------------------------------------------------
//CColor:

class CColor {
    
public:
    CColor(float red, float green, float blue, float alpha = 1);
    CColor(MColor color);
    
    float redComponent  () const;
    float greenComponent() const;
    float blueComponent () const;
    float alphaComponent() const;
    
    MColor color() const;
    bool isClear() const;

    static const CColor blackColor    ;
    static const CColor darkGrayColor ;
    static const CColor grayColor     ;
    static const CColor lightGrayColor;
    static const CColor whiteColor    ;
    static const CColor redColor      ;
    static const CColor greenColor    ;
    static const CColor blueColor     ;
    static const CColor cyanColor     ;
    static const CColor yellowColor   ;
    static const CColor magentaColor  ;
    static const CColor orangeColor   ;
    static const CColor purpleColor   ;
    static const CColor brownColor    ;
    static const CColor clearColor    ;
    
private:
    MColorPattern pattern() const;
    
    MColor mColor;
};

//------------------------------------------------------------------------------
//CImage:

c_class(CImage) {

public:
    static CImageRef fromData  (const std::vector<uint8_t> &data);
    static CImageRef fromBundle(const std::string &path);
    static CImageRef fromFile  (const std::string &path);

    MImage *nativeImage();

private:
    CImage(MImageRef nativeImage);
    
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
void CContextSelectImage (CImageRef          image );
void CContextSelectString(const std::string &string);

void CContextSelectHorizontalAlign(CHorizontalAlign align);
void CContextSelectVerticalAlign  (CVerticalAlign   align);

inline void CContextSetOffset(float x, float y) { MContextSetOffset(x, y); }

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
