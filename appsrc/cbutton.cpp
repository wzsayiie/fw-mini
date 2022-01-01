#include "cbutton.h"

CButton::CButton(float x, float y, float w, float h): CControl(x, y, w, h) {
    _titleColors[CControlState::Normal] = CColor::blackColor;
}

void CButton::setTitle(CControlState state, const std::string &title) {
    _titles[state] = title;
}

void CButton::setTitleColor(CControlState state, const CColor &color) {
    _titleColors[state] = color;
}

void CButton::setFontSize(float size) {
    _fontSize = size;
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
    
    auto title = _titles.find(currentState);
    if (title != _titles.end()) {
        return title->second;
    }
    
    //replaceable title.
    title = _titles.find(CControlState::Normal);
    if (title != _titles.end()) {
        return title->second;
    }
    
    return "";
}

CColor CButton::suitableTitleColor() {
    CControlState currentState = state();
    
    auto color = _titleColors.find(currentState);
    if (color != _titleColors.end()) {
        return color->second;
    }
    
    //replaceable color.
    color = _titleColors.find(CControlState::Normal);
    if (color != _titleColors.end()) {
        
        float r = color->second.redComponent  ();
        float g = color->second.greenComponent();
        float b = color->second.blueComponent ();
        float a = color->second.alphaComponent();
        
        return CColor(r, g, b, a * 0.5);
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
    
    CContextSelectFontSize(_fontSize);
    CContextSelectString(title);
    CContextSelectColor(color);
    
    CContextDrawString(0, 0, width, height);
}
