#pragma once

#include "cview.h"

c_class(CImageView, CView) {
public:
    using base::base;

public:
    void setImage(const MImage::ptr &image);
    MImage::ptr image();

private:
    void onDraw(float width, float height) override;

private:
    MImage::ptr mImage;
};
