#pragma once

//which compiler.
#if _MSC_VER
    #define MCPL_CL    1
    #define MCPL_CLANG 0
    #define MCPL_GCC   0
#elif __clang__
    #define MCPL_CL    0
    #define MCPL_CLANG 1
    #define MCPL_GCC   0
#elif __GNUC__
    #define MCPL_CL    0
    #define MCPL_CLANG 0
    #define MCPL_GCC   1
#else
    #error "unknown compiler."
#endif

//how many bits of cpu word.
#if _WIN64 || (__LP64__ || __ILP64__ || __LLP64__)
    #define MPTR_64 1
    #define MPTR_32 0
#elif _WIN32 || (__LP32__ || __ILP32__)
    #define MPTR_64 0
    #define MPTR_32 1
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
    #define MOS_ANDROID 1
    #define MOS_IOS     0
    #define MOS_OSX     0
    #define MOS_WIN32   0
#elif TARGET_OS_IOS
    #define MOS_ANDROID 0
    #define MOS_IOS     1
    #define MOS_OSX     0
    #define MOS_WIN32   0
#elif TARGET_OS_OSX
    #define MOS_ANDROID 0
    #define MOS_IOS     0
    #define MOS_OSX     1
    #define MOS_WIN32   0
#elif _WIN32
    #define MOS_ANDROID 0
    #define MOS_IOS     0
    #define MOS_OSX     0
    #define MOS_WIN32   1
#else
    #error "unknown os platform."
#endif
