#include "mgraphics.h"
#include "rdefine.h"

//pixel scale:

static float sPixelScale = 1.0f;

define_reflectable_function(MSetPixelScale, "args:scale")
void MSetPixelScale(float scale) {
    sPixelScale = scale;
}

define_reflectable_function(MGetPixelScale)
float MGetPixelScale() {
    return sPixelScale;
}

float _MPointFromPixelCaster::operator<<(float px) {
    return px / sPixelScale;
}

float _MPixelFromPointCaster::operator<<(float pt) {
    return pt * sPixelScale;
}

//alignments:

define_reflectable_enum_const(MHAlign, Left  )
define_reflectable_enum_const(MHAlign, Center)
define_reflectable_enum_const(MHAlign, Right )

define_reflectable_enum_const(MVAlign, Top   )
define_reflectable_enum_const(MVAlign, Middle)
define_reflectable_enum_const(MVAlign, Bottom)

//content mode.
define_reflectable_enum_const(MFillMode, CoverByAspectRatio)
define_reflectable_enum_const(MFillMode, CoverByStretch    )
define_reflectable_enum_const(MFillMode, KeepOriginalSize  )
define_reflectable_enum_const(MFillMode, MatchSpaceWidth   )
define_reflectable_enum_const(MFillMode, MatchSpaceHeight  )

//color:

define_reflectable_class_const(MColor, BlackRGBA    )
define_reflectable_class_const(MColor, DarkGrayRGBA )
define_reflectable_class_const(MColor, GrayRGBA     )
define_reflectable_class_const(MColor, LightGrayRGBA)
define_reflectable_class_const(MColor, WhiteRGBA    )
define_reflectable_class_const(MColor, RedRGBA      )
define_reflectable_class_const(MColor, GreenRGBA    )
define_reflectable_class_const(MColor, BlueRGBA     )
define_reflectable_class_const(MColor, CyanRGBA     )
define_reflectable_class_const(MColor, YellowRGBA   )
define_reflectable_class_const(MColor, MagentaRGBA  )
define_reflectable_class_const(MColor, OrangeRGBA   )
define_reflectable_class_const(MColor, PurpleRGBA   )
define_reflectable_class_const(MColor, BrownRGBA    )
define_reflectable_class_const(MColor, ClearRGBA    )

define_reflectable_class_function(MColor, blackColor    , "getter")
define_reflectable_class_function(MColor, darkGrayColor , "getter")
define_reflectable_class_function(MColor, grayColor     , "getter")
define_reflectable_class_function(MColor, lightGrayColor, "getter")
define_reflectable_class_function(MColor, whiteColor    , "getter")
define_reflectable_class_function(MColor, redColor      , "getter")
define_reflectable_class_function(MColor, greenColor    , "getter")
define_reflectable_class_function(MColor, blueColor     , "getter")
define_reflectable_class_function(MColor, cyanColor     , "getter")
define_reflectable_class_function(MColor, yellowColor   , "getter")
define_reflectable_class_function(MColor, magentaColor  , "getter")
define_reflectable_class_function(MColor, orangeColor   , "getter")
define_reflectable_class_function(MColor, purpleColor   , "getter")
define_reflectable_class_function(MColor, brownColor    , "getter")
define_reflectable_class_function(MColor, clearColor    , "getter")

MColor::ptr MColor::blackColor    () { static auto a = MColor::fromRGBA(BlackRGBA    ); return a; }
MColor::ptr MColor::darkGrayColor () { static auto a = MColor::fromRGBA(DarkGrayRGBA ); return a; }
MColor::ptr MColor::grayColor     () { static auto a = MColor::fromRGBA(GrayRGBA     ); return a; }
MColor::ptr MColor::lightGrayColor() { static auto a = MColor::fromRGBA(LightGrayRGBA); return a; }
MColor::ptr MColor::whiteColor    () { static auto a = MColor::fromRGBA(WhiteRGBA    ); return a; }
MColor::ptr MColor::redColor      () { static auto a = MColor::fromRGBA(RedRGBA      ); return a; }
MColor::ptr MColor::greenColor    () { static auto a = MColor::fromRGBA(GreenRGBA    ); return a; }
MColor::ptr MColor::blueColor     () { static auto a = MColor::fromRGBA(BlueRGBA     ); return a; }
MColor::ptr MColor::cyanColor     () { static auto a = MColor::fromRGBA(CyanRGBA     ); return a; }
MColor::ptr MColor::yellowColor   () { static auto a = MColor::fromRGBA(YellowRGBA   ); return a; }
MColor::ptr MColor::magentaColor  () { static auto a = MColor::fromRGBA(MagentaRGBA  ); return a; }
MColor::ptr MColor::orangeColor   () { static auto a = MColor::fromRGBA(OrangeRGBA   ); return a; }
MColor::ptr MColor::purpleColor   () { static auto a = MColor::fromRGBA(PurpleRGBA   ); return a; }
MColor::ptr MColor::brownColor    () { static auto a = MColor::fromRGBA(BrownRGBA    ); return a; }
MColor::ptr MColor::clearColor    () { static auto a = MColor::fromRGBA(ClearRGBA    ); return a; }

MColor::ptr MColor::fromComs(float redCom, float greenCom, float blueCom, float alphaCom) {
    auto obj = MColor::create();
    
    obj->mRGBA.red   = (uint8_t)(redCom   * 255.f);
    obj->mRGBA.green = (uint8_t)(greenCom * 255.f);
    obj->mRGBA.blue  = (uint8_t)(blueCom  * 255.f);
    obj->mRGBA.alpha = (uint8_t)(alphaCom * 255.f);
    
    return obj;
}

MColor::ptr MColor::fromRGBA(int rgba) {
    auto obj = MColor::create();
    obj->mRGBA.rgba = rgba;
    return obj;
}

define_reflectable_class_function(MColor, redCom  , "getter")
define_reflectable_class_function(MColor, greenCom, "getter")
define_reflectable_class_function(MColor, blueCom , "getter")
define_reflectable_class_function(MColor, alphaCom, "getter")

float MColor::redCom  () { return mRGBA.red   / 255.f; }
float MColor::greenCom() { return mRGBA.green / 255.f; }
float MColor::blueCom () { return mRGBA.blue  / 255.f; }
float MColor::alphaCom() { return mRGBA.alpha / 255.f; }

define_reflectable_class_function(MColor, rgba, "getter")
int MColor::rgba() {
    return mRGBA.rgba;
}

define_reflectable_class_function(MColor, opaque, "getter")
bool MColor::opaque() {
    return mRGBA.alpha == 255;
}

define_reflectable_class_function(MColor, translucent, "getter")
bool MColor::translucent() {
    return 0 < mRGBA.alpha && mRGBA.alpha < 255;
}

define_reflectable_class_function(MColor, none, "getter")
bool MColor::none() {
    return mRGBA.alpha == 0;
}
