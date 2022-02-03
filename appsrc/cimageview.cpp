#include "cimageview.h"

CImageView::CImageView(float x, float y, float w, float h): CView(x, y, w, h) {
    mAnimationTask = m_cast_lambda [this]() {
        onNextAnimatingImage();
    };
}

CImageView::~CImageView() {
    MCancelTask(mAnimationTask.get());
}

void CImageView::setImage(const CImageRef &image) {
    stopAnimating();
    mImage = image;
}

void CImageView::setAnimationImages(const std::vector<CImageRef> &images) {
    stopAnimating();
    
    mAnimationImages = images;
    mImage = !images.empty() ? images[0] : nullptr;
}

void CImageView::setAnimationDuration(float seconds) {
    stopAnimating();
    mAnimationDuration = seconds;
}

void CImageView::setAnimationRepeat(int count) {
    stopAnimating();
    mAnimationRepeat = count;
}

void CImageView::startAnimating() {
    if (mAnimating) {
        return;
    }
    if (mAnimationDuration <= 0) {
        return;
    }
    
    auto imageNumber = (int)mAnimationImages.size();
    if (imageNumber <= 1) {
        return;
    }
    
    //show the first image.
    mAnimating = true;
    mImage = mAnimationImages[0];
    
    //the followed images.
    mAnimatingCount = 0;
    MRunEverySeconds(mAnimationDuration / (float)imageNumber, mAnimationTask.get());
}

void CImageView::stopAnimating() {
    if (!mAnimating) {
        return;
    }
    
    MCancelTask(mAnimationTask.get());
    mAnimating = false;
    
    //NOTE:
    //do not reset "mImage", stay on the last image.
}

bool CImageView::animating() {
    return mAnimating;
}

void CImageView::onNextAnimatingImage() {
    mAnimatingCount += 1;
    
    auto num = (int)mAnimationImages.size();
    int  end = num * mAnimationRepeat;
    
    if (end <= 0) {
        //infinite loop.
        mImage = mAnimationImages[mAnimatingCount % num];
    } else if (mAnimatingCount < end) {
        mImage = mAnimationImages[mAnimatingCount % num];
    } else {
        stopAnimating();
    }
}

void CImageView::onDraw(float width, float height) {
    if (mImage) {
        CContextSelectImage(mImage);
        CContextDrawImage(0, 0, width, height);
    }
}
