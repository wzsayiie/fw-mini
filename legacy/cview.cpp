#include "cview.h"

static MLambda *sWindowListener = nullptr;
static CView *sRootView = nullptr;

CView::CView(float x, float y, float width, float height) {
    mX = x;
    mY = y;
    mWidth  = width ;
    mHeight = height;
}

CView::~CView() {
    if (this == sRootView) {
        sRootView = nullptr;
    }
}

void CView::asRootView() {
    if (!sWindowListener) {
        sWindowListener = MLambdaCreate(handleWindowEvent, nullptr);
        MWindowAddListener(sWindowListener);
    }

    sRootView = this;
    setSupersOffset(0, 0);
}

void CView::setOrigin(float x, float y) {
    if (mX == x && mY == y) {
        return;
    }

    mX = x;
    mY = y;

    for (const CViewRef &subview : mSubviews) {
        subview->setSupersOffset(mSupersX + mX, mSupersY + mY);
    }
}

void CView::setSize(float width, float height) {
    if (mWidth == width && mHeight == height) {
        return;
    }
    
    mWidth  = width ;
    mHeight = height;
    
    if (!mSubviews.empty()) {
        onLayoutSubviews(width, height);
    }
}

float CView::windowX() { return mSupersX + mX; }
float CView::windowY() { return mSupersY + mY; }

float CView::x() { return mX; }
float CView::y() { return mY; }

float CView::width () { return mWidth ; }
float CView::height() { return mHeight; }

void CView::setBackgroundColor(const CColor &color) {
    mBackgroundColor = color;
}

CColor CView::backgroundColor() {
    return mBackgroundColor;
}

void CView::setVisible(bool visible) {
    mVisible = visible;
}

bool CView::visible() {
    return mVisible;
}

void CView::setTouchable(bool touchable) {
    mTouchable = touchable;
}

bool CView::touchable() {
    return mTouchable;
}

void CView::setAcceptMouseWheel(bool accept) {
    mAcceptMouseWheel = true;
}

bool CView::acceptMouseWheel() {
    return mAcceptMouseWheel;
}

static void RemoveSubview(std::vector<CViewRef> *subviews, CView *view) {
    for (auto it = subviews->begin(); it != subviews->end(); ++it) {
        if (it->get() == view) {
            subviews->erase(it);
            break;
        }
    }
}

void CView::addSubview(const CViewRef &subview) {
    if (!subview) {
        return;
    }
    if (subview->mSuperview == this) {
        return;
    }

    if (subview->mSuperview) {
        RemoveSubview(&subview->mSuperview->mSubviews, subview.get());
    }
    mSubviews.push_back(subview);
    subview->mSuperview = this;

    subview->setSupersOffset(mSupersX + mX, mSupersY + mY);
}

void CView::removeFromSuperview() {
    if (!mSuperview) {
        return;
    }

    RemoveSubview(&mSuperview->mSubviews, this);
    mSuperview = nullptr;

    setSupersOffset(0, 0);
}

const std::vector<CViewRef> &CView::subviews() {
    return mSubviews;
}

CView *CView::superview() {
    return mSuperview;
}

bool CView::canRespondWindowTouch(float x, float y) {
    if (!mTouchable) {
        return false;
    }
    
    float left   = windowX();
    float top    = windowY();
    float right  = windowX() + mWidth ;
    float bottom = windowY() + mHeight;
    
    if (left <= x && x <= right &&
        top  <= y && y <= bottom)
    {
        return true;
    }
    return false;
}

bool CView::canRespondWindowWheel(float x, float y) {
    if (!mAcceptMouseWheel) {
        return false;
    }
    
    float left   = windowX();
    float top    = windowY();
    float right  = windowX() + mWidth ;
    float bottom = windowY() + mHeight;
    
    if (left <= x && x <= right &&
        top  <= y && y <= bottom)
    {
        return true;
    }
    return false;
}

CUIResponder *CView::findResponder(const CLambda<bool (CUIResponder *)> &fit) {
    //is there a suitable subview.
    for (const CViewRef &subview : mSubviews) {
        CUIResponder *responder = subview->findResponder(fit);
        if (responder) {
            return responder;
        }
    }
    
    //is self suitable.
    if (fit(this)) {
        return this;
    }
    
    return nullptr;
}

void CView::drawViews() {
    if (!mVisible) {
        return;
    }

    float x = windowX();
    float y = windowY();
    CContextPushClip(x, y, mWidth, mHeight);

    //1. background and self.
    CContextSetDrawOffset(x, y);
    onDrawBackground(mWidth, mHeight);
    onDraw(mWidth, mHeight);
    //2. subviews.
    for (const CViewRef &subview : mSubviews) {
        subview->drawViews();
    }
    //3. foreground.
    CContextSetDrawOffset(x, y);
    onDrawForeground(mWidth, mHeight);
    
    CContextPopClip();
}

void CView::onDrawBackground(float width, float height) {
    if (!mBackgroundColor.isClear()) {
        CContextSelectColor(mBackgroundColor);
        CContextDrawRect(0, 0, width, height);
    }
}

void CView::onWindowTouchBegin(float x, float y) {
    float viewX = x - windowX();
    float viewY = y - windowY();
    onTouchBegin(viewX, viewY);
}

void CView::onWindowTouchMove(float x, float y) {
    float viewX = x - windowX();
    float viewY = y - windowY();
    onTouchMove(viewX, viewY);
}

void CView::onWindowTouchEnd(float x, float y) {
    float viewX = x - windowX();
    float viewY = y - windowY();
    onTouchEnd(viewX, viewY);
}

void CView::onWindowMouseWheel(float x, float y, float delta) {
    float viewX = x - windowX();
    float viewY = y - windowY();
    onMouseWheel(viewX, viewY, delta);
}

void CView::onWindowMouseMove(float x, float y) {
    float viewX = x - windowX();
    float viewY = y - windowY();
    onMouseMove(viewX, viewY);
}

void CView::setSupersOffset(float x, float y) {
    if (mSupersX == x && mSupersY == y) {
        return;
    }

    mSupersX = x;
    mSupersY = y;

    for (const CViewRef &subview : mSubviews) {
        subview->setSupersOffset(mSupersX + mX, mSupersY + mY);
    }
}

void CView::handleWindowEvent(MObject *) {
    if (!sRootView) {
        return;
    }

    MWindowEvent event = MWindowCurrentEvent();
    switch (event) {
        case MWindowEvent_Resize: {
            float width  = MWindowWidth ();
            float height = MWindowHeight();
            sRootView->setSize(width, height);
            break;
        }
        case MWindowEvent_Draw: {
            sRootView->drawViews();
            break;
        }

        default:;
    }
}
