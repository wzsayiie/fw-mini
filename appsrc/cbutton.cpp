#include "cbutton.h"

CButton::CButton(float x, float y, float w, float h): CControl(x, y, w, h) {
    mTitleColors[CControlState::Normal] = CColor::blackColor;
}

void CButton::setTitle(CControlState state, const std::string &title) {
    mTitles[state] = title;
}

void CButton::setTitleColor(CControlState state, const CColor &color) {
    mTitleColors[state] = color;
}

void CButton::setFontSize(float size) {
    mFontSize = size;
}

void CButton::onTouchBegin(float x, float y) {
    setState(CControlState::Selected);
    sendEvent(CControlEvent::TouchDown);
}

void CButton::onTouchEnd(float x, float y) {
    setState(CControlState::Normal);
    
    if (x < 0 || width () < x ||
        y < 0 || height() < y)
    {
        sendEvent(CControlEvent::TouchUpOutside);
    }
    else
    {
        sendEvent(CControlEvent::TouchUpInside);
    }
}

std::string CButton::suitableTitle() {
    CControlState currentState = state();
    
    auto title = mTitles.find(currentState);
    if (title != mTitles.end()) {
        return title->second;
    }
    
    //replaceable title.
    title = mTitles.find(CControlState::Normal);
    if (title != mTitles.end()) {
        return title->second;
    }
    
    return "";
}

CColor CButton::suitableTitleColor() {
    CControlState currentState = state();
    
    auto color = mTitleColors.find(currentState);
    if (color != mTitleColors.end()) {
        return color->second;
    }
    
    //replaceable color.
    color = mTitleColors.find(CControlState::Normal);
    if (color != mTitleColors.end()) {
        
        float r = color->second.redComponent  ();
        float g = color->second.greenComponent();
        float b = color->second.blueComponent ();
        float a = color->second.alphaComponent();
        
        return CColor(r, g, b, a * 0.5f);
    }
    
    return CColor::clearColor;
}

void CButton::onDraw(float width, float height) {
    std::string title = suitableTitle();
    if (title.empty()) {
        return;
    }
    
    CColor color = suitableTitleColor();
    if (color.isClear()) {
        return;
    }
    
    CContextSelectHorizontalAlign(CHorizontalAlign::Center);
    CContextSelectVerticalAlign(CVerticalAlign::Center);
    
    CContextSelectFontSize(mFontSize);
    CContextSelectString(title);
    CContextSelectColor(color);
    
    CContextDrawString(0, 0, width, height);
}
