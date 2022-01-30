#include "cmodobjs.h"

CMOD_META(void   , IModNumber, setBool    , (bool    value), value);
CMOD_META(void   , IModNumber, setInt     , (int     value), value);
CMOD_META(void   , IModNumber, setInt64   , (int64_t value), value);
CMOD_META(void   , IModNumber, setFloat   , (float   value), value);
CMOD_META(void   , IModNumber, setDouble  , (double  value), value);
CMOD_META(bool   , IModNumber, boolValue  , ());
CMOD_META(int    , IModNumber, intValue   , ());
CMOD_META(int64_t, IModNumber, int64Value , ());
CMOD_META(float  , IModNumber, floatValue , ());
CMOD_META(double , IModNumber, doubleValue, ());

void CModNumber::setBool  (bool    value) { mInt64 = value; mDouble = value; }
void CModNumber::setInt   (int     value) { mInt64 = value; mDouble = value; }
void CModNumber::setInt64 (int64_t value) { mInt64 = value; mDouble = value; }
void CModNumber::setFloat (float   value) { mInt64 = value; mDouble = value; }
void CModNumber::setDouble(double  value) { mInt64 = value; mDouble = value; }

bool    CModNumber::boolValue  () { return (bool )mInt64 ; }
int     CModNumber::intValue   () { return (int  )mInt64 ; }
int64_t CModNumber::int64Value () { return /*...*/mInt64 ; }
float   CModNumber::floatValue () { return (float)mDouble; }
double  CModNumber::doubleValue() { return /*...*/mDouble; }

CMOD_META(void             , IModString, setChars, (const cmod_char *chars), chars);
CMOD_META(const cmod_char *, IModString, chars   , ());
CMOD_META(int              , IModString, length  , ());

void CModString::setChars(const cmod_char *chars) {
    mString = chars ? chars : CMOD_L "";
}

const cmod_char *CModString::chars() {
    return mString.c_str();
}

int CModString::length() {
    return (int)mString.size();
}
