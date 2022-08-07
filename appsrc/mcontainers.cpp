#include "mcontainers.h"

using reflect::any;

static void MVector_insert   (const MBaseVector::ptr &a, int i, const any &v) { a->_insert     (i, v); }
static void MVector_erase    (const MBaseVector::ptr &a, int i)               { a->_erase      (i)   ; }
static void MVector_push_back(const MBaseVector::ptr &a, const any &v)        { a->_push_back  (v)   ; }
static void MVector_pop_back (const MBaseVector::ptr &a)                      { a->_pop_back   ( )   ; }
static void MVector_clear    (const MBaseVector::ptr &a)                      { a->_clear      ( )   ; }
static int  MVector_size     (const MBaseVector::ptr &a)                      { return a->_size( )   ; }
static any  MVector_at       (const MBaseVector::ptr &a, int i)               { return a->_at  (i)   ; }
static any  MVector_back     (const MBaseVector::ptr &a)                      { return a->_back( )   ; }

define_reflectable_function(MVector_insert   , "ignore")
define_reflectable_function(MVector_erase    , "ignore")
define_reflectable_function(MVector_push_back, "ignore")
define_reflectable_function(MVector_pop_back , "ignore")
define_reflectable_function(MVector_clear    , "ignore")
define_reflectable_function(MVector_size     , "ignore")
define_reflectable_function(MVector_at       , "ignore")
define_reflectable_function(MVector_back     , "ignore")

static void MMap_insert(const MBaseMap::ptr &a, const any &k, const any &v) { a->_insert      (k, v); }
static void MMap_erase (const MBaseMap::ptr &a, const any &k)               { a->_erase       (k)   ; }
static void MMap_clear (const MBaseMap::ptr &a)                             { a->_clear       ( )   ; }
static bool MMap_has   (const MBaseMap::ptr &a, const any &k)               { return a->_has  (k)   ; }
static any  MMap_get   (const MBaseMap::ptr &a, const any &k)               { return a->_get  (k)   ; }
static int  MMap_size  (const MBaseMap::ptr &a)                             { return a->_size ( )   ; }
static void MMap_begin (const MBaseMap::ptr &a)                             { a->_begin       ( )   ; }
static bool MMap_on    (const MBaseMap::ptr &a)                             { return a->_on   ( )   ; }
static void MMap_next  (const MBaseMap::ptr &a)                             { a->_next        ( )   ; }
static any  MMap_key   (const MBaseMap::ptr &a)                             { return a->_key  ( )   ; }
static any  MMap_value (const MBaseMap::ptr &a)                             { return a->_value( )   ; }

define_reflectable_function(MMap_insert, "ignore")
define_reflectable_function(MMap_erase , "ignore")
define_reflectable_function(MMap_clear , "ignore")
define_reflectable_function(MMap_has   , "ignore")
define_reflectable_function(MMap_get   , "ignore")
define_reflectable_function(MMap_size  , "ignore")
define_reflectable_function(MMap_begin , "ignore")
define_reflectable_function(MMap_on    , "ignore")
define_reflectable_function(MMap_next  , "ignore")
define_reflectable_function(MMap_key   , "ignore")
define_reflectable_function(MMap_value , "ignore")

static void MSet_insert(const MBaseSet::ptr &a, const any &v) { a->_insert      (v); }
static void MSet_erase (const MBaseSet::ptr &a, const any &v) { a->_erase       (v); }
static void MSet_clear (const MBaseSet::ptr &a)               { a->_clear       ( ); }
static bool MSet_has   (const MBaseSet::ptr &a, const any &v) { return a->_has  (v); }
static int  MSet_size  (const MBaseSet::ptr &a)               { return a->_size ( ); }
static void MSet_begin (const MBaseSet::ptr &a)               { a->_begin       ( ); }
static bool MSet_on    (const MBaseSet::ptr &a)               { return a->_on   ( ); }
static void MSet_next  (const MBaseSet::ptr &a)               { a->_next        ( ); }
static any  MSet_value (const MBaseSet::ptr &a)               { return a->_value( ); }

define_reflectable_function(MSet_insert, "ignore")
define_reflectable_function(MSet_erase , "ignore")
define_reflectable_function(MSet_clear , "ignore")
define_reflectable_function(MSet_has   , "ignore")
define_reflectable_function(MSet_size  , "ignore")
define_reflectable_function(MSet_begin , "ignore")
define_reflectable_function(MSet_on    , "ignore")
define_reflectable_function(MSet_next  , "ignore")
define_reflectable_function(MSet_value , "ignore")
