#include "cimageview.h"

define_reflectable_class_function(CImageView, setImage, "setter;args:image")
void CImageView::setImage(const MImage::ptr &image) {
    mImage = image;
}

define_reflectable_class_function(CImageView, image, "getter")
MImage::ptr CImageView::image() {
    return mImage;
}

void CImageView::onDraw(float width, float height) {
    if (mImage) {
        MContextSelectImage(mImage);
        MContextDrawImage(0, 0, width, height);
    }
}
