#include "cscrollview.h"
#include <algorithm>

//scroll content view:

def_class(CScrollContentView) : public CView {
    
public:
    using CView::CView;
    
protected:
    CUIResponder *findResponder(CLambda<bool (CUIResponder *)> fit) override;
    void onDrawViews() override;
    
    bool isInVisibleArea(CViewRef subview);
};

CUIResponder *CScrollContentView::findResponder(CLambda<bool (CUIResponder *)> fit) {
    const std::vector<CViewRef> &views = subviews();
    for (const CViewRef &subview : views) {
        //NOTE: invisible view can not be a first responder.
        if (!isInVisibleArea(subview)) {
            continue;
        }
        
        CUIResponder *responder = subview->findResponder(fit);
        if (responder) {
            return responder;
        }
    }
    
    return nullptr;
}

void CScrollContentView::onDrawViews() {
    //expose the protected "onDrawViews" method.
    struct V : CView {
        void DrawViews() { onDrawViews(); }
    };

    const std::vector<CViewRef> &views = subviews();
    for (const CViewRef &subview : views) {
        //NOTE: ignore invisible views.
        if (!isInVisibleArea(subview)) {
            continue;
        }

        ((V *)subview.get())->DrawViews();
    }
}

bool CScrollContentView::isInVisibleArea(CViewRef subview) {
    float left = -x();
    float top  = -y();
    
    if (subview->x() + subview->width () <= left) { return false; }
    if (subview->y() + subview->height() <= top ) { return false; }
    
    float right  = left + superview()->width ();
    float bottom = top  + superview()->height();
    
    if (right  <= subview->x()) { return false; }
    if (bottom <= subview->y()) { return false; }
    
    return true;
}

//scroll view:

CScrollView::CScrollView(float x, float y, float w, float h) : CView(x, y, w, h) {
    setAcceptMouseWheel(true);
    setTouchable(true);
}

void CScrollView::setDelegate(CScrollViewDelegate *delegate) {
    mDelegate = delegate;
}

void CScrollView::addContentSubview(CViewRef subview) {
    if (subview) {
        contentView()->addSubview(subview);
    }
}

CViewRef CScrollView::contentView() {
    if (!mContentView) {
        mContentView = CScrollContentViewRef(new CScrollContentView(0, 0, 0, 0));
        addSubview(mContentView);
    }
    return mContentView;
}

void CScrollView::setContentSize(float w, float h) {
    CViewRef content = contentView();
    content->setSize(w, h);
    
    float oldX = content->x();
    float oldY = content->y();
    float newX = oldX;
    float newY = oldY;
    
    if (oldX + w < width ()) { newX = std::min(0.f, width () - w); }
    if (oldY + h < height()) { newY = std::min(0.f, height() - h); }
    
    if (newX != oldX || newY != oldY) {
        content->setOrigin(newX, newY);
        
        if (mDelegate) {
            mDelegate->onScrollViewScrolling(this, false);
        }
    }
}

float CScrollView::contentWidth () { return contentView()->width (); }
float CScrollView::contentHeight() { return contentView()->height(); }

//NOTE:
//the content offset and the origin of the content view are opposite.

void CScrollView::setContentOffset(float x, float y) {
    bool accepted = tryContentOrigin(-x, -y);
    if (accepted && mDelegate) {
        mDelegate->onScrollViewScrolling(this, false);
    }
}

float CScrollView::contentX() { return -contentView()->x(); }
float CScrollView::contentY() { return -contentView()->y(); }

void CScrollView::onTouchBegin(float x, float y) {
    mBeginOriginX = contentView()->x();
    mBeginOriginY = contentView()->y();
    mBeginTouchX  = x;
    mBeginTouchY  = y;
}

void CScrollView::onTouchMove(float x, float y) {
    float newX = mBeginOriginX - mBeginTouchX + x;
    float newY = mBeginOriginY - mBeginTouchY + y;
    
    bool accepted = tryContentOrigin(newX, newY);
    if (accepted && mDelegate) {
        mDelegate->onScrollViewScrolling(this, true);
    }
}

void CScrollView::onTouchEnd(float x, float y) {
    float newX = mBeginOriginX - mBeginTouchX + x;
    float newY = mBeginOriginY - mBeginTouchY + y;
    
    bool accepted = tryContentOrigin(newX, newY);
    if (accepted && mDelegate) {
        mDelegate->onScrollViewScrolling(this, true);
    }
}

void CScrollView::onMouseWheel(float x, float y, float delta) {
    CViewRef content = contentView();
    
    float cx = content->x();
    float cy = content->y();
    
    if /**/ (content->height() > height()) { cy += delta; }
    else if (content->width () > width ()) { cx += delta; }
    
    bool accepted = tryContentOrigin(cx, cy);
    if (accepted && mDelegate) {
        mDelegate->onScrollViewScrolling(this, true);
    }
}

bool CScrollView::tryContentOrigin(float x, float y) {
    CViewRef content = contentView();
    
    float oldX = content->x();
    float oldY = content->y();
    float conW = content->width ();
    float conH = content->height();
    
    if (x + conW < width()) {
        x = std::min(0.f, width() - conW);
    } else if (x > 0) {
        x = 0;
    }
    
    if (y + conH < height()) {
        y = std::min(0.f, height() - conH);
    } else if (y > 0) {
        y = 0;
    }
    
    if (x != oldX || y != oldY) {
        content->setOrigin(x, y);
        return true;
    } else {
        return false;
    }
}
