#pragma once

#include <vector>
#include "cgraphics.h"
#include "cuiresponder.h"

typedef std::shared_ptr<class CView> CViewRef;

class CView : public CUIResponder {
    
public:
    CView(float x, float y, float width, float height);
    CView();
    
    void setBackgroundColor(CColor color);
    
    void setOrigin(float x, float y);
    void setSize(float width, float height);
    
    void addSubview(CViewRef subview);
    void removeFromSuperview();
    const std::vector<CViewRef> &subviews();
    CView *superview();
    
    void drawViews();
    virtual void drawSize(float width, float height);
    
private:
    CColor mBackgroundColor;
    
    float mX = 0;
    float mY = 0;
    float mWidth  = 0;
    float mHeight = 0;
    
    std::vector<CViewRef> mSubviews;
    CView *mSuperview = nullptr;
};
