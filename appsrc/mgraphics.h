#pragma once

#include "mobject.h"

//pixel scale:
//

M_HOST_CALL void MSetPixelScale(float scale);
float MGetPixelScale();

#define pt_from_px _MPointFromPixelCaster()<<
#define px_from_pt _MPixelFromPointCaster()<<

struct _MPointFromPixelCaster { float operator<<(float px); };
struct _MPixelFromPointCaster { float operator<<(float pt); };

//alignment:
//

declare_reflectable_enum(MHAlign)
enum class MHAlign {
    Left   = 1,
    Center = 2,
    Right  = 3,
};

declare_reflectable_enum(MVAlign)
enum class MVAlign {
    Top    = 1,
    Middle = 2,
    Bottom = 3,
};

//color:
//

union MColorRGBA {
    int rgba = 0;

    struct {
        uint8_t alpha;
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
    };
};

//android and win32 apis use the order of a-r-g-b.
union MColorARGB {
    int argb = 0;

    struct {
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
        uint8_t alpha;
    };
};

//mac apis use the order of a-b-g-r.
union MColorABGR {
    int abgr = 0;

    struct {
        uint8_t red  ;
        uint8_t green;
        uint8_t blue ;
        uint8_t alpha;
    };
};

declare_reflectable_class(MColor)
class MColor : public MExtends<MColor, MObject> {
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

public:
    MColor();
    MColor(float redCom, float greenCom, float blueCom, float alphaCom);
    MColor(int rgba);

    float redCom  ();
    float greenCom();
    float blueCom ();
    float alphaCom();

    int rgba();

    MColor::ptr copy();

private:
    MColorRGBA _rgba;
};
