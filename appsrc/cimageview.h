#pragma once

#include "cview.h"

def_class(CImageView) : public CView {

public:
    CImageView(float x, float y, float width, float height);
    ~CImageView();

    void setImage(const CImageRef &image);
    
    void setAnimationImages(const std::vector<CImageRef> &images);
    void setAnimationDuration(float seconds);
    void setAnimationRepeat(int count);
    
    void startAnimating();
    void stopAnimating();
    bool animating();

protected:
    void onNextAnimatingImage();
    
    void onDraw(float width, float height) override;

private:
    CImageRef mImage;
    
    std::vector<CImageRef> mAnimationImages;
    MLambdaRef mAnimationTask;
    
    float mAnimationDuration = 0;
    int   mAnimationRepeat   = 0;
    bool  mAnimating         = false;
    int   mAnimatingCount    = 0;
};
