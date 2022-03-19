#pragma once

#include "cgraphics.h"
#include "cuiresponder.h"

def_class(CView) : public CUIResponder {
    
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
    CColor backgroundColor();
    
    void setVisible(bool visible);
    bool visible();
    
    void setTouchable(bool touchable);
    bool touchable();
    
    void setAcceptMouseWheel(bool accept);
    bool acceptMouseWheel();

    void addSubview(const CViewRef &subview);
    void removeFromSuperview();
    const std::vector<CViewRef> &subviews();
    CView *superview();
    
    bool canRespondWindowTouch(float x, float y) override;
    bool canRespondWindowWheel(float x, float y) override;
    
    CUIResponder *findResponder(const CLambda<bool (CUIResponder *)> &fit) override;

    virtual void drawViews();
    
protected:
    virtual void onLayoutSubviews(float width, float height) {}
    
    virtual void onDrawBackground(float width, float height);
    virtual void onDraw          (float width, float height) {}
    virtual void onDrawForeground(float width, float height) {}
    
    virtual void onTouchBegin(float x, float y) {}
    virtual void onTouchMove (float x, float y) {}
    virtual void onTouchEnd  (float x, float y) {}
    virtual void onMouseWheel(float x, float y, float delta) {}
    virtual void onMouseMove (float x, float y) {}
    
    void onWindowTouchBegin(float x, float y) override;
    void onWindowTouchMove (float x, float y) override;
    void onWindowTouchEnd  (float x, float y) override;
    void onWindowMouseWheel(float x, float y, float delta) override;
    void onWindowMouseMove (float x, float y) override;

private:
    void setSupersOffset(float x, float y);

    static void handleWindowEvent(MObject *);

    float mSupersX = 0;
    float mSupersY = 0;
    float mX       = 0;
    float mY       = 0;
    float mWidth   = 0;
    float mHeight  = 0;

    CColor mBackgroundColor {0, 0, 0, 0};
    bool mVisible = true;
    
    //views are not touchable by default.
    bool mTouchable = false;
    //views do not accept mouse wheel by default.
    bool mAcceptMouseWheel = false;
    
    std::vector<CViewRef> mSubviews;
    CView *mSuperview = nullptr;
};
