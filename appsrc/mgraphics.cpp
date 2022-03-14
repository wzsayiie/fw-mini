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

define_reflectable_class_function(MColor, redCom)
float MColor::redCom() {
    return _rgba.red / 255.f;
}

define_reflectable_class_function(MColor, greenCom)
float MColor::greenCom() {
    return _rgba.green / 255.f;
}

define_reflectable_class_function(MColor, blueCom)
float MColor::blueCom() {
    return _rgba.blue / 255.f;
}

define_reflectable_class_function(MColor, alphaCom)
float MColor::alphaCom() {
    return _rgba.alpha / 255.f;
}

define_reflectable_class_function(MColor, rgba)
int MColor::rgba() {
    return _rgba.rgba;
}

define_reflectable_class_function(MColor, copy)
MColor::ptr MColor::copy() {
    return MColor::create(_rgba.rgba);
}
