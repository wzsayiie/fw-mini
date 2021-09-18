#pragma once

//which compiler.
#if _MSC_VER
    #define M_CPL_CL    1
    #define M_CPL_CLANG 0
    #define M_CPL_GCC   0
#elif __clang__
    #define M_CPL_CL    0
    #define M_CPL_CLANG 1
    #define M_CPL_GCC   0
#elif __GNUC__
    #define M_CPL_CL    0
    #define M_CPL_CLANG 0
    #define M_CPL_GCC   1
#else
    #error "unknown compiler."
#endif

//how many bits of cpu word.
#if _WIN64 || (__LP64__ || __ILP64__ || __LLP64__)
    #define M_PTR_64 1
    #define M_PTR_32 0
#elif _WIN32 || (__LP32__ || __ILP32__)
    #define M_PTR_64 0
    #define M_PTR_32 1
#else
    #error "unknown cpu word."
#endif

//which of the following os platform.
//
// +---------------------------+ +---------+
// |           posix           | |         |
// | +-------+ +-------------+ | | windows |
// | |       | |     mac     | | | +-----+ |
// | |android| | +---+ +---+ | | | |     | |
// | |       | | |ios| |osx| | | | |win32| |
// | |       | | +---+ +---+ | | | |     | |
// | +-------+ +-------------+ | | +-----+ |
// +---------------------------+ +---------+
//
#if __APPLE__
    #include <TargetConditionals.h>
#endif
#if ANDROID
    #define M_OS_ANDROID 1
    #define M_OS_IOS     0
    #define M_OS_OSX     0
    #define M_OS_WIN32   0
#elif TARGET_OS_IOS
    #define M_OS_ANDROID 0
    #define M_OS_IOS     1
    #define M_OS_OSX     0
    #define M_OS_WIN32   0
#elif TARGET_OS_OSX
    #define M_OS_ANDROID 0
    #define M_OS_IOS     0
    #define M_OS_OSX     1
    #define M_OS_WIN32   0
#elif _WIN32
    #define M_OS_ANDROID 0
    #define M_OS_IOS     0
    #define M_OS_OSX     0
    #define M_OS_WIN32   1
#else
    #error "unknown os platform."
#endif
