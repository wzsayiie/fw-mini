#pragma once

#include "cview.h"

typedef std::shared_ptr<class CImageView> CImageViewRef;

class CImageView : public CView {

public:
    using CView::CView;

    void setImage(CImageRef image);

protected:
    void onDraw(float width, float height) override;

private:
    CImageRef mImage;
};
