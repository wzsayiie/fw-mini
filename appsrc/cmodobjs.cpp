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

CMOD_META(const cmod_char *, IModString, chars , ());
CMOD_META(int              , IModString, length, ());

CMOD_META(void     , IModArray, resize, (int cnt)              , cnt     );
CMOD_META(int      , IModArray, count , ()                               );
CMOD_META(void     , IModArray, insert, (int idx, IModObj *elm), idx, elm);
CMOD_META(void     , IModArray, remove, (int idx)              , idx     );
CMOD_META(void     , IModArray, set   , (int idx, IModObj *elm), idx, elm);
CMOD_META(IModObj *, IModArray, get   , (int idx)              , idx     );
CMOD_META(void     , IModArray, push  , (IModObj *elm)         , elm     );
CMOD_META(void     , IModArray, pop   , ()                               );
