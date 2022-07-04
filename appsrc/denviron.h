#pragma once

//which compiler.
#if __clang__
    #define D_COMPILER_CLANG 1
    #define D_COMPILER_MSC   0
    #define D_COMPILER_GCC   0
#elif _MSC_VER
    #define D_COMPILER_CLANG 0
    #define D_COMPILER_MSC   1
    #define D_COMPILER_GCC   0
#elif __GNUC__
    #define D_COMPILER_CLANG 0
    #define D_COMPILER_MSC   0
    #define D_COMPILER_GCC   1
#else
    #error "unknown compiler."
#endif

//how many bits is a cpu word.
#if _WIN64 || (__ILP64__ || __LP64__ || __LLP64__)
    #define D_PTR_64 1
    #define D_PTR_32 0
#elif _WIN32 || (__ILP32__ || __LP32__)
    #define D_PTR_64 0
    #define D_PTR_32 1
#else
    #error "unknown cpu word."
#endif

//which os platform.
//
// +---------------------------------------+ +---------+
// |                   posix               | | windows |
// | +-------------------+ +-------------+ | |         |
// | |     linux os      | |     mac     | | | +-----+ |
// | | +-------+ +-----+ | | +---+ +---+ | | | |     | |
// | | |android| |linux| | | |ios| |osx| | | | |win32| |
// | | +-------+ +-----+ | | +---+ +---+ | | | |     | |
// | +-------------------+ +-------------+ | | +-----+ |
// +---------------------------------------+ +---------+
//
#if __APPLE__
    #include <TargetConditionals.h>
#endif
#if __ANDROID__
    #define D_OS_ANDROID 1
    #define D_OS_IOS     0
    #define D_OS_WIN32   0
    #define D_OS_OSX     0
    #define D_OS_LINUX   0
#elif TARGET_OS_IOS
    #define D_OS_ANDROID 0
    #define D_OS_IOS     1
    #define D_OS_WIN32   0
    #define D_OS_OSX     0
    #define D_OS_LINUX   0
#elif _WIN32
    #define D_OS_ANDROID 0
    #define D_OS_IOS     0
    #define D_OS_WIN32   1
    #define D_OS_OSX     0
    #define D_OS_LINUX   0
#elif TARGET_OS_OSX
    #define D_OS_ANDROID 0
    #define D_OS_IOS     0
    #define D_OS_WIN32   0
    #define D_OS_OSX     1
    #define D_OS_LINUX   0
#elif __linux__
    #define D_OS_ANDROID 0
    #define D_OS_IOS     0
    #define D_OS_WIN32   0
    #define D_OS_OSX     0
    #define D_OS_LINUX   1
#else
    #error "unknown os platform."
#endif
