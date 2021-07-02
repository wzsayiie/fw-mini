#pragma once

#include <vector>
#include "cgraphics.h"
#include "cuiresponder.h"

typedef std::shared_ptr<class CView> CViewRef;

class CView : public CUIResponder {
    
public:
    CView(float x, float y, float width, float height);
    ~CView();

    void asRootView();
    
    void setOrigin(float x, float y);
    void setSize(float width, float height);

    float windowX();
    float windowY();
    float x();
    float y();
    float width ();
    float height();
    
    void setBackgroundColor(const CColor &color);
    
    void setVisible(bool visible);
    bool visible();

    void addSubview(CViewRef subview);
    void removeFromSuperview();
    const std::vector<CViewRef> &subviews();
    CView *superview();

protected:
    virtual void onDrawBackground(float width, float height);
    virtual void onDraw(float width, float height) {}

private:
    void setSupersOffset(float x, float y);

    static void handleWindowEvent(MObject *, MObject *);

    void handleResize(float width, float height);
    void handleDraw();

    float mSupersX = 0;
    float mSupersY = 0;
    float mX       = 0;
    float mY       = 0;
    float mWidth   = 0;
    float mHeight  = 0;

    CColor mBackgroundColor {1, 1, 1};
    bool mVisible = true;
    
    std::vector<CViewRef> mSubviews;
    CView *mSuperview = nullptr;
};
