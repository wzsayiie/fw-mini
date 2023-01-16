#include "mgeneric.h"
#include "rdefine.h"

using reflect::any;

//vector:

static MGenericVector::ptr MVector_create() {
    return MGenericVector::create();
}

static void MVector_insert   (const MGenericVector::ptr &a, int i, const any &v) { return a->_insert   (i, v); }
static void MVector_erase    (const MGenericVector::ptr &a, int i)               { return a->_erase    (i)   ; }
static void MVector_push_back(const MGenericVector::ptr &a, const any &v)        { return a->_push_back(v)   ; }
static void MVector_pop_back (const MGenericVector::ptr &a)                      { return a->_pop_back ()    ; }
static void MVector_clear    (const MGenericVector::ptr &a)                      { return a->_clear    ()    ; }
static int  MVector_size     (const MGenericVector::ptr &a)                      { return a->_size     ()    ; }
static any  MVector_at       (const MGenericVector::ptr &a, int i)               { return a->_at       (i)   ; }
static any  MVector_back     (const MGenericVector::ptr &a)                      { return a->_back     ()    ; }

define_reflectable_function(MVector_create   , "ignore")
define_reflectable_function(MVector_insert   , "ignore")
define_reflectable_function(MVector_erase    , "ignore")
define_reflectable_function(MVector_push_back, "ignore")
define_reflectable_function(MVector_pop_back , "ignore")
define_reflectable_function(MVector_clear    , "ignore")
define_reflectable_function(MVector_size     , "ignore")
define_reflectable_function(MVector_at       , "ignore")
define_reflectable_function(MVector_back     , "ignore")

//map:

static MGenericMap::ptr MMap_create() {
    return MGenericMap::create();
}

static void MMap_insert(const MGenericMap::ptr &a, const any &k, const any &v) { return a->_insert(k, v); }
static void MMap_erase (const MGenericMap::ptr &a, const any &k)               { return a->_erase (k)   ; }
static void MMap_clear (const MGenericMap::ptr &a)                             { return a->_clear ()    ; }
static bool MMap_has   (const MGenericMap::ptr &a, const any &k)               { return a->_has   (k)   ; }
static any  MMap_at    (const MGenericMap::ptr &a, const any &k)               { return a->_at    (k)   ; }
static int  MMap_size  (const MGenericMap::ptr &a)                             { return a->_size  ()    ; }
static void MMap_begin (const MGenericMap::ptr &a)                             { return a->_begin ()    ; }
static bool MMap_on    (const MGenericMap::ptr &a)                             { return a->_on    ()    ; }
static void MMap_next  (const MGenericMap::ptr &a)                             { return a->_next  ()    ; }
static any  MMap_key   (const MGenericMap::ptr &a)                             { return a->_key   ()    ; }
static any  MMap_value (const MGenericMap::ptr &a)                             { return a->_value ()    ; }

define_reflectable_function(MMap_create, "ignore")
define_reflectable_function(MMap_insert, "ignore")
define_reflectable_function(MMap_erase , "ignore")
define_reflectable_function(MMap_clear , "ignore")
define_reflectable_function(MMap_has   , "ignore")
define_reflectable_function(MMap_at    , "ignore")
define_reflectable_function(MMap_size  , "ignore")
define_reflectable_function(MMap_begin , "ignore")
define_reflectable_function(MMap_on    , "ignore")
define_reflectable_function(MMap_next  , "ignore")
define_reflectable_function(MMap_key   , "ignore")
define_reflectable_function(MMap_value , "ignore")

//set:

static MGenericSet::ptr MSet_create() {
    return MGenericSet::create();
}

static void MSet_insert(const MGenericSet::ptr &a, const any &v) { return a->_insert(v); }
static void MSet_erase (const MGenericSet::ptr &a, const any &v) { return a->_erase (v); }
static void MSet_clear (const MGenericSet::ptr &a)               { return a->_clear () ; }
static bool MSet_has   (const MGenericSet::ptr &a, const any &v) { return a->_has   (v); }
static int  MSet_size  (const MGenericSet::ptr &a)               { return a->_size  () ; }
static void MSet_begin (const MGenericSet::ptr &a)               { return a->_begin () ; }
static bool MSet_on    (const MGenericSet::ptr &a)               { return a->_on    () ; }
static void MSet_next  (const MGenericSet::ptr &a)               { return a->_next  () ; }
static any  MSet_value (const MGenericSet::ptr &a)               { return a->_value () ; }

define_reflectable_function(MSet_create, "ignore")
define_reflectable_function(MSet_insert, "ignore")
define_reflectable_function(MSet_erase , "ignore")
define_reflectable_function(MSet_clear , "ignore")
define_reflectable_function(MSet_has   , "ignore")
define_reflectable_function(MSet_size  , "ignore")
define_reflectable_function(MSet_begin , "ignore")
define_reflectable_function(MSet_on    , "ignore")
define_reflectable_function(MSet_next  , "ignore")
define_reflectable_function(MSet_value , "ignore")
