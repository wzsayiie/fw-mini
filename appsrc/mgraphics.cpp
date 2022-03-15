#include "mgraphics.h"

//pixel scale:
//

static float sPixelScale = 1.0f;

define_reflectable_function(MSetPixelScale)
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

//alignment:
//

define_reflectable_enum_const(MHAlign, Left  )
define_reflectable_enum_const(MHAlign, Center)
define_reflectable_enum_const(MHAlign, Right )

define_reflectable_enum_const(MVAlign, Top   )
define_reflectable_enum_const(MVAlign, Middle)
define_reflectable_enum_const(MVAlign, Bottom)

//color:
//

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

define_reflectable_class_function(MColor, blackColor    )
define_reflectable_class_function(MColor, darkGrayColor )
define_reflectable_class_function(MColor, grayColor     )
define_reflectable_class_function(MColor, lightGrayColor)
define_reflectable_class_function(MColor, whiteColor    )
define_reflectable_class_function(MColor, redColor      )
define_reflectable_class_function(MColor, greenColor    )
define_reflectable_class_function(MColor, blueColor     )
define_reflectable_class_function(MColor, cyanColor     )
define_reflectable_class_function(MColor, yellowColor   )
define_reflectable_class_function(MColor, magentaColor  )
define_reflectable_class_function(MColor, orangeColor   )
define_reflectable_class_function(MColor, purpleColor   )
define_reflectable_class_function(MColor, brownColor    )
define_reflectable_class_function(MColor, clearColor    )

const MColor::ptr &MColor::blackColor    () { static auto a = MColor::create(BlackRGBA    ); return a; }
const MColor::ptr &MColor::darkGrayColor () { static auto a = MColor::create(DarkGrayRGBA ); return a; }
const MColor::ptr &MColor::grayColor     () { static auto a = MColor::create(GrayRGBA     ); return a; }
const MColor::ptr &MColor::lightGrayColor() { static auto a = MColor::create(LightGrayRGBA); return a; }
const MColor::ptr &MColor::whiteColor    () { static auto a = MColor::create(WhiteRGBA    ); return a; }
const MColor::ptr &MColor::redColor      () { static auto a = MColor::create(RedRGBA      ); return a; }
const MColor::ptr &MColor::greenColor    () { static auto a = MColor::create(GreenRGBA    ); return a; }
const MColor::ptr &MColor::blueColor     () { static auto a = MColor::create(BlueRGBA     ); return a; }
const MColor::ptr &MColor::cyanColor     () { static auto a = MColor::create(CyanRGBA     ); return a; }
const MColor::ptr &MColor::yellowColor   () { static auto a = MColor::create(YellowRGBA   ); return a; }
const MColor::ptr &MColor::magentaColor  () { static auto a = MColor::create(MagentaRGBA  ); return a; }
const MColor::ptr &MColor::orangeColor   () { static auto a = MColor::create(OrangeRGBA   ); return a; }
const MColor::ptr &MColor::purpleColor   () { static auto a = MColor::create(PurpleRGBA   ); return a; }
const MColor::ptr &MColor::brownColor    () { static auto a = MColor::create(BrownRGBA    ); return a; }
const MColor::ptr &MColor::clearColor    () { static auto a = MColor::create(ClearRGBA    ); return a; }

MColor::MColor() {
}

MColor::MColor(float redCom, float greenCom, float blueCom, float alphaCom) {
    _rgba.red   = (uint8_t)(redCom   * 255.f);
    _rgba.green = (uint8_t)(greenCom * 255.f);
    _rgba.blue  = (uint8_t)(blueCom  * 255.f);
    _rgba.alpha = (uint8_t)(alphaCom * 255.f);
}

MColor::MColor(int rgba) {
    _rgba.rgba = rgba;
}

define_reflectable_class_function(MColor, redCom  )
define_reflectable_class_function(MColor, greenCom)
define_reflectable_class_function(MColor, blueCom )
define_reflectable_class_function(MColor, alphaCom)

float MColor::redCom  () { return _rgba.red   / 255.f; }
float MColor::greenCom() { return _rgba.green / 255.f; }
float MColor::blueCom () { return _rgba.blue  / 255.f; }
float MColor::alphaCom() { return _rgba.alpha / 255.f; }

define_reflectable_class_function(MColor, rgba)
int MColor::rgba() {
    return _rgba.rgba;
}

define_reflectable_class_function(MColor, copy)
MColor::ptr MColor::copy() {
    return MColor::create(_rgba.rgba);
}
