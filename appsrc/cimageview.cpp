#include "cimageview.h"
#include "mcontext.h"
#include "rdefine.h"

define_reflectable_class_function(CImageView, setImage   , "setter;args:image")
define_reflectable_class_function(CImageView, setFillMode, "setter;args:mode" )
define_reflectable_class_function(CImageView, setHAlign  , "setter;args:align")
define_reflectable_class_function(CImageView, setVAlign  , "setter;args:align")

void CImageView::setImage   (const MImage::ptr &image) { mImage    = image; }
void CImageView::setFillMode(MFillMode          mode ) { mFillMode = mode ; }
void CImageView::setHAlign  (MHAlign            align) { mHAlign   = align; }
void CImageView::setVAlign  (MVAlign            align) { mVAlign   = align; }

define_reflectable_class_function(CImageView, image   , "getter")
define_reflectable_class_function(CImageView, fillMode, "getter")
define_reflectable_class_function(CImageView, hAlign  , "getter")
define_reflectable_class_function(CImageView, vAlign  , "getter")

MImage::ptr CImageView::image   () { return mImage   ; }
MFillMode   CImageView::fillMode() { return mFillMode; }
MHAlign     CImageView::hAlign  () { return mHAlign  ; }
MVAlign     CImageView::vAlign  () { return mVAlign  ; }

void CImageView::onDraw(float aw, float ah) {
    if (!mImage) {
        return;
    }

    float iw = mImage->pixelSize()->width ();
    float ih = mImage->pixelSize()->height();

    float w = 0;
    float h = 0;
    switch (mFillMode) {
        case MFillMode::CoverByAspectRatio: {
            //try match space width.
            {
                w = aw;
                h = ih / iw * aw;
            }
            //try match space height.
            if (h < ah) {
                w = iw / ih * ah;
                h = ah;
            }
            break;
        }
        case MFillMode::CoverByStretch  : w = aw      ; h = ah      ; break;
        case MFillMode::KeepOriginalSize: w = iw      ; h = ih      ; break;
        case MFillMode::MatchSpaceWidth : w = aw      ; h = ih/iw*aw; break;
        case MFillMode::MatchSpaceHeight: w = iw/ih*ah; h = ah      ; break;
        default:;
    }

    float x = 0;
    switch (mHAlign) {
        case MHAlign::Left  : x = (0)         ; break;
        case MHAlign::Center: x = (aw - w) / 2; break;
        case MHAlign::Right : x = (aw - w)    ; break;
        default:;
    }

    float y = 0;
    switch (mVAlign) {
        case MVAlign::Top   : y = (0)         ; break;
        case MVAlign::Middle: y = (ah - h) / 2; break;
        case MVAlign::Bottom: y = (ah - h)    ; break;
        default:;
    }

    MContextSelectImage(mImage);
    MContextDrawImage(x, y, w, h);
}
