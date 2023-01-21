#pragma once

#include "cview.h"

m_class(CControl, CView) {
public:
    CControl();
    CControl(float x, float y, float width, float height);
    
    ~CControl();
    
public:
    void setControlId(const std::string &iden);
    std::string controlId();
    
    void        setPreviousControl(const std::string &iden);
    void        setNextControl    (const std::string &iden);
    std::string previousControl   ();
    std::string nextControl       ();

    void transferFocusToPrevious();
    void transferFocusToNext    ();
    void transferFocusToAny     ();
    
protected:
    void onDrawForeground(float width, float height) override;
    void onKbKey(MKbKeyCode code) override final;
    
protected: public:
    virtual void onControlKbKey(MKbKeyCode code);
    
private:
    CControl::ptr checkoutControl(const std::string &iden);
    
private:
    std::string mControlId      ;
    std::string mPreviousControl;
    std::string mNextControl    ;
};
