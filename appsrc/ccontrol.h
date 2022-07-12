#pragma once

#include "cview.h"

c_class(CControl, CView) {
public:
    CControl();
    CControl(float x, float y, float width, float height);
    
    ~CControl();
    
public:
    void setControlId(const std::string &iden);
    std::string controlId();
    
    void transferFocusControl();
    
    void setFrontControl(const std::string &iden);
    void setNextControl (const std::string &iden);
    void setLeftControl (const std::string &iden);
    void setRightControl(const std::string &iden);
    
    std::string frontControl();
    std::string nextControl ();
    std::string leftControl ();
    std::string rightControl();
    
protected:
    void onDrawForeground(float width, float height) override;
    void onKey(MKey key) override final;
    
protected: public:
    virtual void onControlKey(MKey key);
    
private:
    void setControl(CControl **target, const std::string &iden);
    std::string controlIdOf(CControl **target);

    void transfer(const std::initializer_list<CControl **> &ids);
    
private:
    std::string mControlId;
    
    CControl *mFrontControl = nullptr;
    CControl *mNextControl  = nullptr;
    CControl *mLeftControl  = nullptr;
    CControl *mRightControl = nullptr;
};
