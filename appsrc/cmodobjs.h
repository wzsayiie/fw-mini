#pragma once

#include "cmodmeta.h"

cmod_intf(IModNumber, IModObj) {

    virtual void setBool  (bool    value);
    virtual void setInt   (int     value);
    virtual void setInt64 (int64_t value);
    virtual void setFloat (float   value);
    virtual void setDouble(double  value);

    virtual bool    boolValue  ();
    virtual int     intValue   ();
    virtual int64_t int64Value ();
    virtual float   floatValue ();
    virtual double  doubleValue();
};

class CModNumber : public IModNumber {
    
public:
    void setBool  (bool    value) override;
    void setInt   (int     value) override;
    void setInt64 (int64_t value) override;
    void setFloat (float   value) override;
    void setDouble(double  value) override;

    bool    boolValue  () override;
    int     intValue   () override;
    int64_t int64Value () override;
    float   floatValue () override;
    double  doubleValue() override;
    
private:
    int64_t mInt64  = 0;
    double  mDouble = 0;
};

cmod_intf(IModString, IModObj) {

    virtual void setChars(const cmod_char *chars);
    virtual const cmod_char *chars();
    virtual int length();
};

class CModString : public IModString {
    
public:
    void setChars(const cmod_char *chars) override;
    const cmod_char *chars() override;
    int length() override;
    
private:
    cmod_string mString;
};
