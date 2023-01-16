#pragma once

#include "cview.h"
#include "mimage.h"

m_class(CImageView, CView) {
public:
    using super::super;

public:
    void setImage   (const MImage::ptr &image);
    void setFillMode(MFillMode          mode );
    void setHAlign  (MHAlign            align);
    void setVAlign  (MVAlign            align);

    MImage::ptr image   ();
    MFillMode   fillMode();
    MHAlign     hAlign  ();
    MVAlign     vAlign  ();

protected:
    void onDraw(float width, float height) override;

private:
    MImage::ptr mImage    ;
    MFillMode   mFillMode = MFillMode::CoverByAspectRatio;
    MHAlign     mHAlign   = MHAlign::Center;
    MVAlign     mVAlign   = MVAlign::Middle;
};
