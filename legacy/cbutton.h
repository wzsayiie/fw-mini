#pragma once

#include "ccontrol.h"

def_class(CButton) : public CControl {
    
public:
    CButton(float x, float y, float width, float height);
    
    void setTitle(CControlState state, const std::string &title);
    void setTitleColor(CControlState state, const CColor &color);
    void setFontSize(float size);
    
protected:
    void onTouchBegin(float x, float y) override;
    void onTouchEnd  (float x, float y) override;
    
    void onDraw(float width, float height) override;
    
    std::string suitableTitle();
    CColor suitableTitleColor();
    
private:
    std::map<CControlState, std::string> mTitles;
    std::map<CControlState, CColor> mTitleColors;
    float mFontSize = 16;
};
