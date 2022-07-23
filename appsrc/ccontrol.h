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
    
    void setPreviousControl(const std::string &iden);
    void setNextControl    (const std::string &iden);
    
    std::string previousControl();
    std::string nextControl    ();

    void transferFocusControl();
    
protected:
    void onDrawForeground(float width, float height) override;
    void onKbKey(MKbKeyCode code) override final;
    
protected: public:
    virtual void onControlKbKey(MKbKeyCode code);
    
private:
    void setControl(CControl **target, const std::string &iden);
    std::string controlIdOf(CControl **target);

    void transfer(const std::initializer_list<CControl **> &targets);
    
private:
    std::string mControlId;
    
    CControl *mPreviousControl = nullptr;
    CControl *mNextControl     = nullptr;
};
