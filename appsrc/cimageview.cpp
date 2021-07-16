#include "cimageview.h"

void CImageView::setImage(CImageRef image) {
    mImage = image;
}

void CImageView::onDraw(float width, float height) {
    if (mImage) {
        CContextSelectImage(mImage);
        CContextDrawImage(0, 0, width, height);
    }
}
