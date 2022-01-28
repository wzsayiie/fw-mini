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

cmod_intf(IModString, IModObj) {

    virtual const cmod_char *chars();
    virtual int length();
};

cmod_intf(IModArray, IModObj) {

    virtual void resize(int count);
    virtual int count();

    virtual void     insert(int index, IModObj *elem);
    virtual void     remove(int index);
    virtual void     set   (int index, IModObj *elem);
    virtual IModObj *get   (int index);

    virtual void push(IModObj *elem);
    virtual void pop();
};
