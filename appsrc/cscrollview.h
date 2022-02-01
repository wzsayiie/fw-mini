#pragma once

#include "cview.h"

class CScrollView;

//scroll view delegate:

struct CScrollViewDelegate {
    virtual ~CScrollViewDelegate() {}
    virtual void onScrollViewScrolling(CScrollView *scrollView, bool dragged) {}
};

//scroll view:

def_class(CScrollView) : public CView {
    
public:
    CScrollView(float x, float y, float width, float height);
    
    void setDelegate(CScrollViewDelegate *delegate);
    
    void addContentSubview(CViewRef subview);
    CViewRef contentView();
    
    void setContentSize(float width, float height);
    float contentWidth();
    float contentHeight();
    
    void setContentOffset(float x, float y);
    float contentX();
    float contentY();
    
protected:
    void onTouchBegin(float x, float y) override;
    void onTouchMove (float x, float y) override;
    void onTouchEnd  (float x, float y) override;
    
    void onMouseWheel(float x, float y, float delta) override;
    
private:
    bool tryContentOrigin(float x, float y);
    
    CScrollViewDelegate *mDelegate = nullptr;
    CViewRef mContentView;
    
    float mBeginOriginX = 0;
    float mBeginOriginY = 0;
    float mBeginTouchX  = 0;
    float mBeginTouchY  = 0;
};
