#pragma once

#include "mobject.h"

//pixel scale:

M_HOST_CALL void MSetPixelScale(float scale);
float MGetPixelScale();

#define m_pt_from_px _MPointFromPixelCaster()<<
#define m_px_from_pt _MPixelFromPointCaster()<<

struct _MPointFromPixelCaster { float operator<<(float px); };
struct _MPixelFromPointCaster { float operator<<(float pt); };

//alignment:

m_enum(MHAlign) {
    Left   = 1,
    Center = 2,
    Right  = 3,
};

m_enum(MVAlign) {
    Top    = 1,
    Middle = 2,
    Bottom = 3,
};

//color:

union MColorRGBA {
    int rgba = 0;

    struct {
        uint8_t alpha;
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
    };
};

//android and win32 gdi+ use the order of Argb.
union MEarlyARGB {
    int argb = 0;

    struct {
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
        uint8_t alpha;
    };
};

//mac apis use the order of Abgr.
union MClassicABGR {
    int abgr = 0;

    struct {
        uint8_t red  ;
        uint8_t green;
        uint8_t blue ;
        uint8_t alpha;
    };
};

template<class Src, class Dst> void MColorTransform(Src *src, Dst *dst, int count) {
    for (; count-- > 0; ++src, ++dst) {
        //NOTE: src and dst may point to a same position.
        Src item = *src;
        
        dst->red   = item.red  ;
        dst->green = item.green;
        dst->blue  = item.blue ;
        dst->alpha = item.alpha;
    }
}
template<class T> void MColorTransform(T *src, T *dst, int count) {
}

m_class(MColor, MObject) {
public:
    static const int BlackRGBA     = 0x000000ff;
    static const int DarkGrayRGBA  = 0x404040ff;
    static const int GrayRGBA      = 0x808080ff;
    static const int LightGrayRGBA = 0xC0C0C0ff;
    static const int WhiteRGBA     = 0xFFFFFFff;
    static const int RedRGBA       = 0xC3272Bff;
    static const int GreenRGBA     = 0x21A675ff;
    static const int BlueRGBA      = 0x177CB0ff;
    static const int CyanRGBA      = 0x4C8DAEff;
    static const int YellowRGBA    = 0xF2BE45ff;
    static const int MagentaRGBA   = 0xA01DAEff;
    static const int OrangeRGBA    = 0xFFA400ff;
    static const int PurpleRGBA    = 0x8D4BBBff;
    static const int BrownRGBA     = 0xE29C45ff;
    static const int ClearRGBA     = 0x00000000;

    static const MColor::ptr &blackColor    ();
    static const MColor::ptr &darkGrayColor ();
    static const MColor::ptr &grayColor     ();
    static const MColor::ptr &lightGrayColor();
    static const MColor::ptr &whiteColor    ();
    static const MColor::ptr &redColor      ();
    static const MColor::ptr &greenColor    ();
    static const MColor::ptr &blueColor     ();
    static const MColor::ptr &cyanColor     ();
    static const MColor::ptr &yellowColor   ();
    static const MColor::ptr &magentaColor  ();
    static const MColor::ptr &orangeColor   ();
    static const MColor::ptr &purpleColor   ();
    static const MColor::ptr &brownColor    ();
    static const MColor::ptr &clearColor    ();

public:
    static MColor::ptr fromComs(float redCom, float greenCom, float blueCom, float alphaCom);
    static MColor::ptr fromRGBA(int rgba);

public:
    float redCom  ();
    float greenCom();
    float blueCom ();
    float alphaCom();
    int   rgba    ();

    bool opaque();      //alpha is 1.
    bool translucent(); //alpha is between (0, 1).
    bool none();        //alpha is 0.

private:
    MColorRGBA mRGBA;
};
