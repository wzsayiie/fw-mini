#pragma once

#include "cview.h"

centui_class(CImageView, CView) {
public:
    using upper::upper;

public:
    void setImage(const MImage::ptr &image);
    MImage::ptr image();

private:
    void onDraw(float width, float height) override;

private:
    MImage::ptr mImage;
};
