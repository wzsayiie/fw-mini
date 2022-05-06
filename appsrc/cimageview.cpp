#include "cimageview.h"

define_reflectable_class_function(CImageView, setImage, "args:image")
void CImageView::setImage(const MImage::ptr &image) {
    mImage = image;
}

define_reflectable_class_function(CImageView, image)
MImage::ptr CImageView::image() {
    return mImage;
}

void CImageView::onDraw(float width, float height) {
    if (mImage) {
        MContextSelectImage(mImage);
        MContextDrawImage(0, 0, width, height);
    }
}
