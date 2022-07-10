#pragma once

#include "cview.h"

c_class(CControl, CView) {
public:
    CControl();
    CControl(float x, float y, float width, float height);
    
    ~CControl();
    
public:
    void setResponderId(const std::string &iden);
    std::string responderId();
    
    void transferFocusResponder();
    
    void setFrontResponder(const std::string &iden);
    void setNextResponder (const std::string &iden);
    void setLeftResponder (const std::string &iden);
    void setRightResponder(const std::string &iden);
    
    std::string frontResponder();
    std::string nextResponder ();
    std::string leftResponder ();
    std::string rightResponder();
    
protected:
    void onDrawForeground(float width, float height) override;
    
    bool canRespondKey() override;
    void onKey(MKey key) override final;
    
protected: public:
    virtual void onControlKey(MKey key);
    
private:
    void transfer(const std::initializer_list<std::string> &ids);
    
private:
    std::string mResponderId;
    
    std::string mFrontResponder;
    std::string mNextResponder ;
    std::string mLeftResponder ;
    std::string mRightResponder;
};
