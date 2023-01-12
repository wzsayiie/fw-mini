#pragma once

#include "cview.h"

m_class(CImageView, CView) {
public:
    using super::super;

public:
    void setImage(const MImage::ptr &image);
    MImage::ptr image();

protected:
    void onDraw(float width, float height) override;

private:
    MImage::ptr mImage;
};
